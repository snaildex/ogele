//
// Created by ??????? on 24.08.2018.
//

#ifndef OGELE_CLOUDS_H
#define OGELE_CLOUDS_H
namespace ogele {

    class Clouds : public Object {

        ivec3 m_resolution;

        unique_ptr<Texture3D> m_density;
        unique_ptr<Texture3D> m_noise;
        unique_ptr<Material> m_material;
        ShaderProgram* m_cloudsGen;
        ShaderProgram* m_cloudNoiseGen;

    public:

        Clouds(const ivec3 &resolution);

        inline const ivec3 &GetResolution() const noexcept { return m_resolution; }

        inline Material* GetMaterial() const noexcept { return m_material.get(); }

        void Generate();

    };
}
#endif //OGELE_CLOUDS_H
