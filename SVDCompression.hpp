#ifndef SVD_COMPRESSION_H
#define SVD_COMPRESSION_H

#include "Image.hpp"
#include "eigen/Eigen/Dense"
#include "eigen/Eigen/SVD"

class SVDCompression {
public:
    static void compressImage(Image* img, int k);
    static void decompressImage(Image* img, const Eigen::MatrixXd& U, const Eigen::VectorXd& S, const Eigen::MatrixXd& V, int width, int height);
};

#endif // SVD_COMPRESSION_H
