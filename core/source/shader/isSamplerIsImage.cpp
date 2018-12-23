#include <stdafx.h>
#include <ogele.h>
using namespace std;
namespace ogele {
    bool ShaderProgram::IsSampler(UniformType type) {
        return (
                type == UniformType::Sampler1D ||
                type == UniformType::Sampler1DArray ||
                type == UniformType::Sampler1DArrayShadow ||
                type == UniformType::Sampler1DShadow ||
                type == UniformType::Sampler2D ||
                type == UniformType::Sampler2DArray ||
                type == UniformType::Sampler2DArrayShadow ||
                type == UniformType::Sampler2DMultisample ||
                type == UniformType::Sampler2DMultisampleArray ||
                type == UniformType::Sampler2DRect ||
                type == UniformType::Sampler2DRectShadow ||
                type == UniformType::Sampler2DShadow ||
                type == UniformType::Sampler3D ||
                type == UniformType::SamplerCube ||
                type == UniformType::SamplerCubeArray ||
                type == UniformType::SamplerCubeShadow ||
                type == UniformType::SamplerCubeArrayShadow ||
                type == UniformType::SamplerBuffer ||

                type == UniformType::IntSampler1D ||
                type == UniformType::IntSampler1DArray ||
                type == UniformType::IntSampler2D ||
                type == UniformType::IntSampler2DArray ||
                type == UniformType::IntSampler2DMultisample ||
                type == UniformType::IntSampler2DMultisampleArray ||
                type == UniformType::IntSampler2DRect ||
                type == UniformType::IntSampler3D ||
                type == UniformType::IntSamplerCube ||
                type == UniformType::IntSamplerCubeArray ||
                type == UniformType::IntSamplerBuffer ||

                type == UniformType::UnsignedIntSampler1D ||
                type == UniformType::UnsignedIntSampler1DArray ||
                type == UniformType::UnsignedIntSampler2D ||
                type == UniformType::UnsignedIntSampler2DArray ||
                type == UniformType::UnsignedIntSampler2DMultisample ||
                type == UniformType::UnsignedIntSampler2DMultisampleArray ||
                type == UniformType::UnsignedIntSampler2DRect ||
                type == UniformType::UnsignedIntSampler3D ||
                type == UniformType::UnsignedIntSamplerCube ||
                type == UniformType::UnsignedIntSamplerCubeArray ||
                type == UniformType::UnsignedIntSamplerBuffer
        );
    }

    bool ShaderProgram::IsImage(UniformType type) {
        return (
                type == UniformType::Image1D ||
                type == UniformType::Image1DArray ||
                type == UniformType::Image2D ||
                type == UniformType::Image2DArray ||
                type == UniformType::Image2DMultisample ||
                type == UniformType::Image2DMultisampleArray ||
                type == UniformType::Image2DRect ||
                type == UniformType::Image3D ||
                type == UniformType::ImageCube ||
                type == UniformType::ImageCubeArray ||

                type == UniformType::IntImage1D ||
                type == UniformType::IntImage1DArray ||
                type == UniformType::IntImage2D ||
                type == UniformType::IntImage2DArray ||
                type == UniformType::IntImage2DMultisample ||
                type == UniformType::IntImage2DMultisampleArray ||
                type == UniformType::IntImage2DRect ||
                type == UniformType::IntImage3D ||
                type == UniformType::UnsignedIntImage1D ||
                type == UniformType::UnsignedIntImage1DArray ||
                type == UniformType::UnsignedIntImage2D ||
                type == UniformType::UnsignedIntImage2DArray ||
                type == UniformType::UnsignedIntImage2DMultisample ||
                type == UniformType::UnsignedIntImage2DMultisampleArray ||
                type == UniformType::UnsignedIntImage2DRect ||
                type == UniformType::UnsignedIntImage3D ||
                type == UniformType::UnsignedIntImageCube ||
                type == UniformType::UnsignedIntImageCubeArray
        );
    }

}