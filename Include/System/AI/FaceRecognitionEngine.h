/// \file FaceRecognitionEngine.h
/// \brief Provides local machine-learning face detection, 128-D embedding extraction, and identity matching.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure, IEEE 754 Floating-Point Arithmetic, ITU-R BT.601.

#pragma once

#include "Common.h"
#include "System/Array.h"
#include "System/Collections/Generic/List.h"
#include "System/Drawing/Imaging/RawImage.h"
#include "System/Object.h"
#include "System/SmartPointer.h"

namespace DotNetDupe {
    namespace System {
        namespace AI {

            /// \class DetectedFace
            /// \brief Represents a detected face region and its 128-dimensional L2-normalized feature embedding.
            ///
            /// Encapsulates pixel-space bounding box coordinates, detection confidence in [0.0, 1.0],
            /// and a unit-norm 128-D float embedding vector suitable for cosine similarity clustering.
            ///
            /// \note Conforms to ECMA-335 Partition I Section 8.9 (System.Object) and IEEE 754.
            /// \see FaceRecognitionEngine, IFaceRecognitionBackend, VectorMath
            class DetectedFace : public Object {
            public:
                /// \brief Initializes an empty detected face descriptor with a zeroed 128-D embedding.
                DOTNETDUPE_API DetectedFace();

                /// \brief Initializes a detected face descriptor with bounding box, confidence, and embedding.
                /// \param iX Left X coordinate in pixels.
                /// \param iY Top Y coordinate in pixels.
                /// \param iWidth Bounding box width in pixels.
                /// \param iHeight Bounding box height in pixels.
                /// \param fConfidence Detection confidence in [0.0, 1.0].
                /// \param arrEmbedding The 128-D L2-normalized face embedding vector.
                DOTNETDUPE_API DetectedFace(
                    int iX,
                    int iY,
                    int iWidth,
                    int iHeight,
                    float fConfidence,
                    const Array<float>& arrEmbedding);

                /// \brief Virtual destructor.
                DOTNETDUPE_API ~DetectedFace() override;

                /// \brief Gets the left X coordinate of the face bounding box in pixels.
                /// \return Left X coordinate in pixels.
                DOTNETDUPE_API int GetX() const noexcept;

                /// \brief Gets the top Y coordinate of the face bounding box in pixels.
                /// \return Top Y coordinate in pixels.
                DOTNETDUPE_API int GetY() const noexcept;

                /// \brief Gets the width of the face bounding box in pixels.
                /// \return Bounding box width in pixels.
                DOTNETDUPE_API int GetWidth() const noexcept;

                /// \brief Gets the height of the face bounding box in pixels.
                /// \return Bounding box height in pixels.
                DOTNETDUPE_API int GetHeight() const noexcept;

                /// \brief Gets the detection confidence score in [0.0, 1.0].
                /// \return Detection confidence value.
                DOTNETDUPE_API float GetConfidence() const noexcept;

                /// \brief Gets the 128-dimensional L2-normalized embedding vector for this face.
                /// \return Const reference to the 128-element float embedding array.
                DOTNETDUPE_API const Array<float>& GetEmbedding() const noexcept;

            private:
                int m_iX;
                int m_iY;
                int m_iWidth;
                int m_iHeight;
                float m_fConfidence;
                Array<float> m_arrEmbedding;
            };

            /// \class IFaceRecognitionBackend
            /// \brief Pluggable backend interface for face detection and 128-D feature embedding extraction.
            ///
            /// Enables applications and unit tests to inject custom neural inference backends (e.g., ONNX Runtime,
            /// TensorRT, or deterministic test mocks) into FaceRecognitionEngine.
            ///
            /// \note Conforms to ECMA-335 interface contract semantics.
            /// \see FaceRecognitionEngine, DetectedFace
            class IFaceRecognitionBackend : public Object {
            public:
                /// \brief Virtual destructor.
                DOTNETDUPE_API ~IFaceRecognitionBackend() override = default;

                /// \brief Detects all faces in the given image and computes their 128-D feature embeddings.
                /// \param pImage The decoded ARGB image to inspect.
                /// \return A list of detected faces and their embeddings.
                /// \throws ArgumentNullException Thrown when pImage is null.
                virtual Collections::Generic::List<DetectedFace> DetectAndEmbedFaces(
                    const SmartPointer<Drawing::Imaging::RawImage>& pImage) = 0;

                /// \brief Computes a 128-D L2-normalized embedding for a specified rectangular region of an image.
                /// \param pImage The decoded ARGB image.
                /// \param iX Left X coordinate of the face region.
                /// \param iY Top Y coordinate of the face region.
                /// \param iWidth Width of the face region.
                /// \param iHeight Height of the face region.
                /// \return A 128-element L2-normalized float embedding array.
                /// \throws ArgumentNullException Thrown when pImage is null.
                /// \throws ArgumentOutOfRangeException Thrown when the region falls outside the image bounds.
                virtual Array<float> ComputeRegionEmbedding(
                    const SmartPointer<Drawing::Imaging::RawImage>& pImage,
                    int iX,
                    int iY,
                    int iWidth,
                    int iHeight) = 0;
            };

            /// \class FaceRecognitionEngine
            /// \brief Provides local machine-learning face detection, 128-D embedding extraction, and identity matching.
            ///
            /// Ships with a built-in deterministic spatial-chromatic / ITU-R BT.601 luminance-gradient backend
            /// that operates 100% offline with zero external runtime dependencies, while supporting pluggable
            /// custom backends via IFaceRecognitionBackend.
            ///
            /// \note Conforms to ECMA-335, IEEE 754, and ITU-R BT.601 color-space luminance standards.
            /// \see DetectedFace, IFaceRecognitionBackend, VectorMath
            class FaceRecognitionEngine : public Object {
            public:
                /// \brief Standard dimensionality (128 floats) of face embeddings produced by the engine.
                static constexpr int EmbeddingDimension = 128;

                /// \brief Initializes the face recognition engine with the built-in local multi-scale neural/spatial backend.
                DOTNETDUPE_API FaceRecognitionEngine();

                /// \brief Initializes the face recognition engine with a custom pluggable backend.
                /// \param pBackend The custom backend implementation (must not be null).
                /// \throws ArgumentNullException Thrown when pBackend is null.
                DOTNETDUPE_API explicit FaceRecognitionEngine(const SmartPointer<IFaceRecognitionBackend>& pBackend);

                /// \brief Virtual destructor.
                DOTNETDUPE_API ~FaceRecognitionEngine() override;

                /// \brief Replaces the active face recognition backend.
                /// \param pBackend The non-null backend to install.
                /// \throws ArgumentNullException Thrown when pBackend is null.
                DOTNETDUPE_API void SetBackend(const SmartPointer<IFaceRecognitionBackend>& pBackend);

                /// \brief Gets the currently configured face recognition backend.
                /// \return SmartPointer to the active IFaceRecognitionBackend instance.
                DOTNETDUPE_API SmartPointer<IFaceRecognitionBackend> GetBackend() const noexcept;

                /// \brief Detects faces in the provided image and computes 128-D L2-normalized embeddings for each face.
                /// \param pImage The ARGB image to analyze.
                /// \return A list of detected faces sorted by confidence descending.
                /// \throws ArgumentNullException Thrown when pImage is null.
                DOTNETDUPE_API Collections::Generic::List<DetectedFace> DetectAndEmbedFaces(
                    const SmartPointer<Drawing::Imaging::RawImage>& pImage);

                /// \brief Computes a 128-D L2-normalized face embedding for an explicit bounding box region.
                /// \param pImage The ARGB image containing the face.
                /// \param iX Left X coordinate of the face region.
                /// \param iY Top Y coordinate of the face region.
                /// \param iWidth Width of the face region.
                /// \param iHeight Height of the face region.
                /// \return A 128-element L2-normalized float embedding array.
                /// \throws ArgumentNullException Thrown when pImage is null.
                /// \throws ArgumentOutOfRangeException Thrown when the region coordinates or dimensions are invalid.
                DOTNETDUPE_API Array<float> ComputeFaceRegionEmbedding(
                    const SmartPointer<Drawing::Imaging::RawImage>& pImage,
                    int iX,
                    int iY,
                    int iWidth,
                    int iHeight);

                /// \brief Computes the cosine similarity score in [-1.0, 1.0] between two face embeddings.
                /// \param arrEmbeddingA The first face embedding.
                /// \param arrEmbeddingB The second face embedding.
                /// \return The cosine similarity score.
                /// \throws ArgumentException Thrown when embeddings are empty or differ in length.
                static DOTNETDUPE_API float ComputeSimilarity(
                    const Array<float>& arrEmbeddingA,
                    const Array<float>& arrEmbeddingB);

                /// \brief Determines whether two face embeddings represent the same person given a similarity threshold.
                /// \param arrEmbeddingA The first face embedding.
                /// \param arrEmbeddingB The second face embedding.
                /// \param fThreshold The minimum cosine similarity threshold in (0.0, 1.0] (default 0.75).
                /// \return True if similarity is greater than or equal to fThreshold; otherwise false.
                /// \throws ArgumentOutOfRangeException Thrown when fThreshold is not in (0.0, 1.0].
                /// \throws ArgumentException Thrown when embeddings are empty or differ in length.
                static DOTNETDUPE_API bool IsSamePerson(
                    const Array<float>& arrEmbeddingA,
                    const Array<float>& arrEmbeddingB,
                    float fThreshold = 0.75f);

            private:
                SmartPointer<IFaceRecognitionBackend> m_pBackend;
            };

        }
    }
}
