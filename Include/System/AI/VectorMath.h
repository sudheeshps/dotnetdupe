/// \file VectorMath.h
/// \brief Provides high-performance vector mathematics operations for machine learning embeddings.
///
/// Standard Citation: IEEE 754 Floating-Point Arithmetic, ECMA-335 CLI Common Language Infrastructure.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"

namespace DotNetDupe {
    namespace System {
        namespace AI {

            /// \class VectorMath
            /// \brief Provides static vector arithmetic and similarity metrics for floating-point embeddings.
            ///
            /// Accumulates inner products and squared norms in IEEE 754 double precision to prevent
            /// catastrophic cancellation across high-dimensional embedding vectors (e.g., 128-D or 512-D).
            /// Thread-safe: all public methods are stateless and reentrant.
            ///
            /// \note Conforms to IEEE 754 Standard for Floating-Point Arithmetic and ECMA-335 Partition IV.
            /// \see FaceRecognitionEngine, DetectedFace
            class VectorMath : public Object {
            public:
                /// \brief Computes the inner dot product of two float vectors of equal length.
                /// \param arrLeft The first vector (must be non-empty).
                /// \param arrRight The second vector (must match arrLeft length).
                /// \return The scalar dot product of the two vectors.
                /// \throws ArgumentException Thrown when either vector is empty or dimensions do not match.
                /// \note Uses IEEE 754 64-bit accumulator before casting to 32-bit float.
                static DOTNETDUPE_API float DotProduct(const Array<float>& arrLeft, const Array<float>& arrRight);

                /// \brief Computes the Euclidean L2 norm (magnitude) of a float vector.
                /// \param arrVector The input float vector (must be non-empty).
                /// \return The non-negative L2 magnitude of the vector.
                /// \throws ArgumentException Thrown when arrVector is empty.
                /// \note Computes sqrt(sum(x_i^2)) in double precision per IEEE 754.
                static DOTNETDUPE_API float Magnitude(const Array<float>& arrVector);

                /// \brief Returns a new L2-normalized unit vector with magnitude 1.0 (or zero vector if magnitude is zero).
                /// \param arrVector The input float vector to normalize.
                /// \return A normalized copy of the input vector on the unit hypersphere.
                /// \throws ArgumentException Thrown when arrVector is empty.
                static DOTNETDUPE_API Array<float> L2Normalize(const Array<float>& arrVector);

                /// \brief Computes the cosine similarity in the range [-1.0, 1.0] between two float vectors.
                /// \param arrLeft The first embedding vector.
                /// \param arrRight The second embedding vector.
                /// \return The cosine similarity score clamped to [-1.0f, 1.0f], or 0.0f if either vector has zero magnitude.
                /// \throws ArgumentException Thrown when either vector is empty or dimensions do not match.
                static DOTNETDUPE_API float CosineSimilarity(const Array<float>& arrLeft, const Array<float>& arrRight);

                /// \brief Computes the Euclidean (L2) distance between two float vectors of equal length.
                /// \param arrLeft The first embedding vector.
                /// \param arrRight The second embedding vector.
                /// \return The non-negative Euclidean distance between the two vectors.
                /// \throws ArgumentException Thrown when either vector is empty or dimensions do not match.
                static DOTNETDUPE_API float EuclideanDistance(const Array<float>& arrLeft, const Array<float>& arrRight);
            };

        }
    }
}
