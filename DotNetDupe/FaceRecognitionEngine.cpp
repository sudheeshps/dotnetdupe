#include "pch.h"
#include "System/AI/FaceRecognitionEngine.h"
#include "System/AI/VectorMath.h"
#include "System/ArgumentNullException.h"
#include "System/ArgumentOutOfRangeException.h"
#include <cmath>

namespace DotNetDupe {
    namespace System {
        namespace AI {

            namespace {
                /// \brief Samples a normalized RGB and luminance descriptor at a fractional (u, v) coordinate within a region.
                static void SampleNormalizedPixel(
                    const Drawing::Imaging::RawImage& img,
                    int iRectX,
                    int iRectY,
                    int iRectW,
                    int iRectH,
                    float fU,
                    float fV,
                    float& refR,
                    float& refG,
                    float& refB,
                    float& refLuma) {
                    /// Clamp fractional coordinates to valid image bounds.
                    const int iClampedX = iRectX + static_cast<int>(fU * static_cast<float>(iRectW - 1));
                    const int iClampedY = iRectY + static_cast<int>(fV * static_cast<float>(iRectH - 1));
                    const int iSafeX = (iClampedX < 0) ? 0 : ((iClampedX >= img.GetWidth()) ? (img.GetWidth() - 1) : iClampedX);
                    const int iSafeY = (iClampedY < 0) ? 0 : ((iClampedY >= img.GetHeight()) ? (img.GetHeight() - 1) : iClampedY);

                    /// Unpack 32-bit ARGB channels and compute ITU-R BT.601 relative luminance.
                    const unsigned int uArgb = img.GetPixelArgb(iSafeX, iSafeY);
                    refR = static_cast<float>((uArgb >> 16) & 0xFFU) / 255.0f;
                    refG = static_cast<float>((uArgb >> 8) & 0xFFU) / 255.0f;
                    refB = static_cast<float>(uArgb & 0xFFU) / 255.0f;
                    refLuma = 0.299f * refR + 0.587f * refG + 0.114f * refB;
                }

                /// \brief Computes a 4-feature spatial cell descriptor at grid cell (iGridX, iGridY).
                static void ComputeCellDescriptor(
                    const Drawing::Imaging::RawImage& img,
                    int iRectX,
                    int iRectY,
                    int iRectW,
                    int iRectH,
                    int iGridX,
                    int iGridY,
                    Array<float>& arrOut,
                    int iBaseIdx) {
                    /// Sample center and adjacent horizontal points for the grid cell.
                    const float fU = (static_cast<float>(iGridX) + 0.5f) / 8.0f;
                    const float fV = (static_cast<float>(iGridY) + 0.5f) / 4.0f;
                    float fR = 0.0f, fG = 0.0f, fB = 0.0f, fLuma = 0.0f;
                    SampleNormalizedPixel(img, iRectX, iRectY, iRectW, iRectH, fU, fV, fR, fG, fB, fLuma);
                    float fRightR = 0.0f, fRightG = 0.0f, fRightB = 0.0f, fRightLuma = 0.0f;
                    const float fNextU = (fU + 0.125f <= 1.0f) ? (fU + 0.125f) : 1.0f;
                    SampleNormalizedPixel(img, iRectX, iRectY, iRectW, iRectH, fNextU, fV, fRightR, fRightG, fRightB, fRightLuma);

                    /// Store luminance, opponent color differences, and horizontal gradient.
                    arrOut[iBaseIdx] = fLuma;
                    arrOut[iBaseIdx + 1] = fR - fG;
                    arrOut[iBaseIdx + 2] = fB - fG;
                    arrOut[iBaseIdx + 3] = fRightLuma - fLuma;
                }

                /// \brief Validates that the image and region dimensions are valid for feature extraction.
                static void ValidateRegionArgs(
                    const SmartPointer<Drawing::Imaging::RawImage>& pImage,
                    int iX,
                    int iY,
                    int iWidth,
                    int iHeight) {
                    /// Guard: Ensure image pointer is valid and bounding box lies within image dimensions.
                    if (!pImage) {
                        throw ArgumentNullException("pImage");
                    }
                    if (iWidth <= 0 || iHeight <= 0 || iX < 0 || iY < 0) {
                        throw ArgumentOutOfRangeException("Face region dimensions must be positive and non-negative.");
                    }
                    if (iX + iWidth > pImage->GetWidth() || iY + iHeight > pImage->GetHeight()) {
                        throw ArgumentOutOfRangeException("Face region exceeds image bounds.");
                    }
                }

                /// \brief Evaluates whether a normalized RGB sample exhibits facial skin-tone and contrast characteristics.
                static float ScoreFacialSkinAndContrast(
                    const Drawing::Imaging::RawImage& img,
                    int iRectX,
                    int iRectY,
                    int iRectW,
                    int iRectH) {
                    /// Sample central cheek/nose region and upper eye-line region.
                    float fCenterR = 0.0f, fCenterG = 0.0f, fCenterB = 0.0f, fCenterLuma = 0.0f;
                    SampleNormalizedPixel(img, iRectX, iRectY, iRectW, iRectH, 0.5f, 0.55f, fCenterR, fCenterG, fCenterB, fCenterLuma);
                    float fEyeR = 0.0f, fEyeG = 0.0f, fEyeB = 0.0f, fEyeLuma = 0.0f;
                    SampleNormalizedPixel(img, iRectX, iRectY, iRectW, iRectH, 0.35f, 0.35f, fEyeR, fEyeG, fEyeB, fEyeLuma);

                    /// Combine skin warmth heuristic with eye-to-cheek luminance contrast.
                    const float fWarmth = (fCenterR >= fCenterG && fCenterG >= fCenterB * 0.85f) ? 0.55f : 0.25f;
                    const float fEyeContrast = std::fabs(fCenterLuma - fEyeLuma);
                    const float fRawScore = fWarmth + fEyeContrast * 0.85f;
                    return (fRawScore > 0.99f) ? 0.99f : fRawScore;
                }

                /// \brief Default built-in local spatial-chromatic face detection and 128-D embedding backend.
                class BuiltInFaceRecognitionBackend : public IFaceRecognitionBackend {
                public:
                    Array<float> ComputeRegionEmbedding(
                        const SmartPointer<Drawing::Imaging::RawImage>& pImage,
                        int iX,
                        int iY,
                        int iWidth,
                        int iHeight) override {
                        /// Validate region bounds and extract an 8x4 spatial grid of 4-channel descriptors (128 floats).
                        ValidateRegionArgs(pImage, iX, iY, iWidth, iHeight);
                        Array<float> arrRaw(FaceRecognitionEngine::EmbeddingDimension);
                        for (int iCell = 0; iCell < 32; ++iCell) {
                            ComputeCellDescriptor(*pImage, iX, iY, iWidth, iHeight, iCell % 8, iCell / 8, arrRaw, iCell * 4);
                        }

                        return VectorMath::L2Normalize(arrRaw);
                    }

                    Collections::Generic::List<DetectedFace> DetectAndEmbedFaces(
                        const SmartPointer<Drawing::Imaging::RawImage>& pImage) override {
                        /// Guard: Validate input image pointer.
                        if (!pImage) {
                            throw ArgumentNullException("pImage");
                        }

                        /// Evaluate central candidate portrait region and extract embedding if score exceeds threshold.
                        Collections::Generic::List<DetectedFace> lstFaces;
                        const int iImgW = pImage->GetWidth();
                        const int iImgH = pImage->GetHeight();
                        const int iBoxW = (iImgW > 4) ? (iImgW / 2) : iImgW;
                        const int iBoxH = (iImgH > 4) ? (iImgH / 2) : iImgH;
                        const int iBoxX = (iImgW - iBoxW) / 2;
                        const int iBoxY = (iImgH - iBoxH) / 2;
                        const float fScore = ScoreFacialSkinAndContrast(*pImage, iBoxX, iBoxY, iBoxW, iBoxH);
                        if (fScore >= 0.50f) {
                            const Array<float> arrEmb = ComputeRegionEmbedding(pImage, iBoxX, iBoxY, iBoxW, iBoxH);
                            lstFaces.Add(DetectedFace(iBoxX, iBoxY, iBoxW, iBoxH, fScore, arrEmb));
                        }

                        return lstFaces;
                    }
                };
            }

            DetectedFace::DetectedFace()
                : m_iX(0)
                , m_iY(0)
                , m_iWidth(0)
                , m_iHeight(0)
                , m_fConfidence(0.0f)
                , m_arrEmbedding(FaceRecognitionEngine::EmbeddingDimension) {
                /// Default constructor initializes a zeroed 128-D embedding vector.
            }

            DetectedFace::DetectedFace(
                int iX,
                int iY,
                int iWidth,
                int iHeight,
                float fConfidence,
                const Array<float>& arrEmbedding)
                : m_iX(iX)
                , m_iY(iY)
                , m_iWidth(iWidth)
                , m_iHeight(iHeight)
                , m_fConfidence(fConfidence)
                , m_arrEmbedding(arrEmbedding) {
                /// Parameterized constructor captures bounding box, confidence, and 128-D embedding.
            }

            DetectedFace::~DetectedFace() = default;

            int DetectedFace::GetX() const noexcept {
                /// Return left bounding box coordinate.
                return m_iX;
            }

            int DetectedFace::GetY() const noexcept {
                /// Return top bounding box coordinate.
                return m_iY;
            }

            int DetectedFace::GetWidth() const noexcept {
                /// Return bounding box width.
                return m_iWidth;
            }

            int DetectedFace::GetHeight() const noexcept {
                /// Return bounding box height.
                return m_iHeight;
            }

            float DetectedFace::GetConfidence() const noexcept {
                /// Return detection confidence in [0.0, 1.0].
                return m_fConfidence;
            }

            const Array<float>& DetectedFace::GetEmbedding() const noexcept {
                /// Return immutable reference to the 128-D embedding vector.
                return m_arrEmbedding;
            }

            FaceRecognitionEngine::FaceRecognitionEngine()
                : m_pBackend(SmartPointer<IFaceRecognitionBackend>(
                    SmartPointer<BuiltInFaceRecognitionBackend>::NewShared())) {
                /// Initialize engine with built-in local face recognition backend.
            }

            FaceRecognitionEngine::FaceRecognitionEngine(const SmartPointer<IFaceRecognitionBackend>& pBackend)
                : m_pBackend(pBackend) {
                /// Guard: Validate that the custom backend pointer is non-null.
                if (!m_pBackend) {
                    throw ArgumentNullException("pBackend");
                }
            }

            FaceRecognitionEngine::~FaceRecognitionEngine() = default;

            void FaceRecognitionEngine::SetBackend(const SmartPointer<IFaceRecognitionBackend>& pBackend) {
                /// Guard: Validate backend pointer before replacement.
                if (!pBackend) {
                    throw ArgumentNullException("pBackend");
                }

                m_pBackend = pBackend;
            }

            SmartPointer<IFaceRecognitionBackend> FaceRecognitionEngine::GetBackend() const noexcept {
                /// Return current backend smart pointer.
                return m_pBackend;
            }

            Collections::Generic::List<DetectedFace> FaceRecognitionEngine::DetectAndEmbedFaces(
                const SmartPointer<Drawing::Imaging::RawImage>& pImage) {
                /// Delegate face detection and embedding extraction to the active backend.
                return m_pBackend->DetectAndEmbedFaces(pImage);
            }

            Array<float> FaceRecognitionEngine::ComputeFaceRegionEmbedding(
                const SmartPointer<Drawing::Imaging::RawImage>& pImage,
                int iX,
                int iY,
                int iWidth,
                int iHeight) {
                /// Delegate rectangular face region embedding extraction to the active backend.
                return m_pBackend->ComputeRegionEmbedding(pImage, iX, iY, iWidth, iHeight);
            }

            float FaceRecognitionEngine::ComputeSimilarity(
                const Array<float>& arrEmbeddingA,
                const Array<float>& arrEmbeddingB) {
                /// Compute cosine similarity between two face embeddings.
                return VectorMath::CosineSimilarity(arrEmbeddingA, arrEmbeddingB);
            }

            bool FaceRecognitionEngine::IsSamePerson(
                const Array<float>& arrEmbeddingA,
                const Array<float>& arrEmbeddingB,
                float fThreshold) {
                /// Guard: Validate similarity threshold range.
                if (fThreshold <= 0.0f || fThreshold > 1.0f) {
                    throw ArgumentOutOfRangeException("Similarity threshold must be in (0.0, 1.0].");
                }

                return ComputeSimilarity(arrEmbeddingA, arrEmbeddingB) >= fThreshold;
            }

        }
    }
}
