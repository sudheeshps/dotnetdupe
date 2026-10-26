/// \file RawImage.h
/// \brief Represents an uncompressed 32-bit ARGB pixel buffer with cropping, resizing, and ML tensor conversion.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Array.h"
#include "System/SmartPointer.h"
#include "System/Drawing/Imaging/ExifMetadata.h"

namespace DotNetDupe {
    namespace System {
        namespace Drawing {
            namespace Imaging {

                /// \class RawImage
                /// \brief Encapsulates a 32-bit ARGB raster image buffer (4 bytes per pixel: B, G, R, A in little-endian u32).
                ///
                /// Supports native BMP loading/saving, region cropping, bilinear scaling for thumbnails,
                /// and normalized floating-point RGB tensor extraction for local ML inference.
                ///
                /// \see ExifMetadata
                class RawImage : public Object {
                public:
                    /// \brief Initializes a new RawImage with the specified dimensions.
                    /// \param iWidth Image width in pixels (must be > 0).
                    /// \param iHeight Image height in pixels (must be > 0).
                    /// \throws ArgumentOutOfRangeException Thrown when iWidth or iHeight is <= 0.
                    DOTNETDUPE_API RawImage(int iWidth, int iHeight);

                    /// \brief Virtual destructor.
                    DOTNETDUPE_API ~RawImage() override = default;

                    /// \brief Creates a new zero-initialized RawImage of the specified dimensions.
                    DOTNETDUPE_API static SmartPointer<RawImage> Create(int iWidth, int iHeight);

                    /// \brief Creates a RawImage from an existing 32-bit ARGB byte buffer (length must be iWidth * iHeight * 4).
                    DOTNETDUPE_API static SmartPointer<RawImage> FromArgbBytes(int iWidth, int iHeight, const Array<byte>& arrArgb);

                    /// \brief Loads and decodes an image file from disk into a 32-bit ARGB RawImage.
                    /// \param sFilePath Path to the image file on disk.
                    /// \return SmartPointer to the decoded RawImage.
                    DOTNETDUPE_API static SmartPointer<RawImage> LoadFromFile(const String& sFilePath);

                    /// \brief Saves this RawImage to disk as an uncompressed 32-bit BMP file.
                    /// \param sFilePath Destination BMP file path.
                    DOTNETDUPE_API void SaveToBmp(const String& sFilePath) const;

                    /// \brief Gets the image width in pixels.
                    DOTNETDUPE_API int GetWidth() const noexcept;

                    /// \brief Gets the image height in pixels.
                    DOTNETDUPE_API int GetHeight() const noexcept;

                    /// \brief Gets the underlying 32-bit ARGB byte buffer (iWidth * iHeight * 4 bytes).
                    DOTNETDUPE_API const Array<byte>& GetPixels() const noexcept;

                    /// \brief Gets the associated EXIF and header metadata.
                    DOTNETDUPE_API const ExifMetadata& GetExifMetadata() const noexcept;

                    /// \brief Sets the associated EXIF and header metadata.
                    DOTNETDUPE_API void SetExifMetadata(const ExifMetadata& metadata);

                    /// \brief Gets the 32-bit ARGB pixel color at (iX, iY) as 0xAARRGGBB.
                    DOTNETDUPE_API unsigned int GetPixelArgb(int iX, int iY) const noexcept;

                    /// \brief Sets the 32-bit ARGB pixel color at (iX, iY) as 0xAARRGGBB.
                    DOTNETDUPE_API void SetPixelArgb(int iX, int iY, unsigned int uArgb) noexcept;

                    /// \brief Extracts a rectangular sub-region as a new RawImage.
                    DOTNETDUPE_API SmartPointer<RawImage> Crop(int iX, int iY, int iCropWidth, int iCropHeight) const;

                    /// \brief Resizes this image to new dimensions using bilinear interpolation.
                    DOTNETDUPE_API SmartPointer<RawImage> ResizeBilinear(int iNewWidth, int iNewHeight) const;

                    /// \brief Resizes the image and converts pixels into a normalized [0.0f, 1.0f] RGB float tensor (size: iTargetWidth * iTargetHeight * 3).
                    DOTNETDUPE_API Array<float> ToNormalizedRgbTensor(int iTargetWidth, int iTargetHeight) const;

                private:
                    int m_iWidth;
                    int m_iHeight;
                    Array<byte> m_arrPixels;
                    ExifMetadata m_exif;
                };

            }
        }
    }
}
