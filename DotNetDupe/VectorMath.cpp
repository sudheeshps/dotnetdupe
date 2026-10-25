#include "pch.h"
#include "System/AI/VectorMath.h"
#include "System/ArgumentException.h"
#include <cmath>

namespace DotNetDupe {
    namespace System {
        namespace AI {

            namespace {
                /// \brief Validates that two vectors are non-empty and have matching dimensions.
                static void ValidateVectorPair(const Array<float>& arrLeft, const Array<float>& arrRight) {
                    if (arrLeft.GetLength() <= 0 || arrRight.GetLength() <= 0) {
                        throw ArgumentException("Vectors must not be empty.");
                    }
                    if (arrLeft.GetLength() != arrRight.GetLength()) {
                        throw ArgumentException("Vector dimensions must match.");
                    }
                }
            }

            float VectorMath::DotProduct(const Array<float>& arrLeft, const Array<float>& arrRight) {
                /// Guard: Validate dimensions before accumulating the inner product.
                ValidateVectorPair(arrLeft, arrRight);

                /// Accumulate element-wise products in double precision.
                double dSum = 0.0;
                for (int iIndex = 0; iIndex < arrLeft.GetLength(); ++iIndex) {
                    dSum += static_cast<double>(arrLeft[iIndex]) * static_cast<double>(arrRight[iIndex]);
                }

                return static_cast<float>(dSum);
            }

            float VectorMath::Magnitude(const Array<float>& arrVector) {
                /// Guard: Validate that the vector contains at least one element.
                if (arrVector.GetLength() <= 0) {
                    throw ArgumentException("Vector must not be empty.");
                }

                /// Sum squared components and compute Euclidean L2 norm.
                double dSquaredSum = 0.0;
                for (int iIndex = 0; iIndex < arrVector.GetLength(); ++iIndex) {
                    const double dVal = static_cast<double>(arrVector[iIndex]);
                    dSquaredSum += dVal * dVal;
                }

                return static_cast<float>(std::sqrt(dSquaredSum));
            }

            Array<float> VectorMath::L2Normalize(const Array<float>& arrVector) {
                /// Compute L2 magnitude and allocate normalized output vector.
                const float fMag = Magnitude(arrVector);
                Array<float> arrNormalized(arrVector.GetLength());

                /// Scale each component by the magnitude when non-zero.
                for (int iIndex = 0; iIndex < arrVector.GetLength(); ++iIndex) {
                    arrNormalized[iIndex] = (fMag > 1e-12f) ? (arrVector[iIndex] / fMag) : 0.0f;
                }

                return arrNormalized;
            }

            float VectorMath::CosineSimilarity(const Array<float>& arrLeft, const Array<float>& arrRight) {
                /// Guard: Validate vector pair and check for zero-magnitude vectors.
                ValidateVectorPair(arrLeft, arrRight);
                const float fLeftMag = Magnitude(arrLeft);
                const float fRightMag = Magnitude(arrRight);
                if (fLeftMag <= 1e-12f || fRightMag <= 1e-12f) {
                    return 0.0f;
                }

                /// Compute cosine of angle from dot product and clamp to [-1.0, 1.0].
                const float fDot = DotProduct(arrLeft, arrRight);
                const float fCos = fDot / (fLeftMag * fRightMag);
                return (fCos > 1.0f) ? 1.0f : ((fCos < -1.0f) ? -1.0f : fCos);
            }

            float VectorMath::EuclideanDistance(const Array<float>& arrLeft, const Array<float>& arrRight) {
                /// Guard: Validate vector pair dimensions.
                ValidateVectorPair(arrLeft, arrRight);

                /// Sum squared element-wise differences and take square root.
                double dSquaredDiff = 0.0;
                for (int iIndex = 0; iIndex < arrLeft.GetLength(); ++iIndex) {
                    const double dDiff = static_cast<double>(arrLeft[iIndex]) - static_cast<double>(arrRight[iIndex]);
                    dSquaredDiff += dDiff * dDiff;
                }

                return static_cast<float>(std::sqrt(dSquaredDiff));
            }

        }
    }
}
