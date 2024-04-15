#include "SVDCompression.hpp"

void SVDCompression::compressImage(Image* img, int k) {
    // Assuming Image uses matrices from the Eigen library
    Eigen::MatrixXd R = Eigen::MatrixXd::Zero(img->height, img->width);
    Eigen::MatrixXd G = Eigen::MatrixXd::Zero(img->height, img->width);
    Eigen::MatrixXd B = Eigen::MatrixXd::Zero(img->height, img->width);

    // Fill matrices with image data
    for (int i = 0; i < img->height; ++i) {
        for (int j = 0; j < img->width; ++j) {
            Pixel px = Image_get_pixel(img, i, j);
            R(i, j) = px.r;
            G(i, j) = px.g;
            B(i, j) = px.b;
        }
    }

    // Perform SVD on each channel
    Eigen::JacobiSVD<Eigen::MatrixXd> svdR(R, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::JacobiSVD<Eigen::MatrixXd> svdG(G, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::JacobiSVD<Eigen::MatrixXd> svdB(B, Eigen::ComputeThinU | Eigen::ComputeThinV);

    // Reduce dimensions according to k
    auto Ur = svdR.matrixU().leftCols(k);
    auto Sr = svdR.singularValues().head(k).asDiagonal();
    auto Vr = svdR.matrixV().leftCols(k);

    auto Ug = svdG.matrixU().leftCols(k);
    auto Sg = svdG.singularValues().head(k).asDiagonal();
    auto Vg = svdG.matrixV().leftCols(k);

    auto Ub = svdB.matrixU().leftCols(k);
    auto Sb = svdB.singularValues().head(k).asDiagonal();
    auto Vb = svdB.matrixV().leftCols(k);

    // Optionally: store U, S, V for each channel to allow decompression or further processing
}

void SVDCompression::decompressImage(Image* img, const Eigen::MatrixXd& U, const Eigen::VectorXd& S, const Eigen::MatrixXd& V, int width, int height) {
    Eigen::MatrixXd R = U * S * V.transpose();
    // similarly for G and B

    // Update img with decompressed data
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            Pixel px;
            px.r = static_cast<unsigned char>(std::min(std::max(0.0, R(i, j)), 255.0));
            px.g = static_cast<unsigned char>(std::min(std::max(0.0, R(i, j)), 255.0)); // Replace R with G for green
            px.b = static_cast<unsigned char>(std::min(std::max(0.0, R(i, j)), 255.0)); // Replace R with B for blue
            Image_set_pixel(img, i, j, px);
        }
    }
}
