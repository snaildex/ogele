#pragma once
namespace ogele {

	class Camera : public Component {
		glm::dvec3 m_eulerAngles;
		glm::ivec2 m_frameSize;
	protected:
		glm::dmat4 m_projMatrix;
		std::unique_ptr<Material> m_material;

		virtual void UpdateProjection() {};
	protected:
		Camera(Actor* actor, const glm::ivec2 &frameSize);

	public:
		void LookAround(const glm::dvec2 &delta, double speed, double xMin, double xMax);

		void SetFrameSize(const glm::ivec2 &value) noexcept {
			m_frameSize = value;
			UpdateProjection();
		}

		virtual Component* Clone(Actor* actor) const = 0;

		virtual void UpdateMaterial();
		virtual std::string GetName() const override { return "Camera"; };

		const glm::ivec2 &GetFrameSize() const noexcept { return m_frameSize; }
		const float GetAspectRatio() const noexcept { return m_frameSize.x / (float)m_frameSize.y; }
		glm::dmat4 GetViewMatrix() const noexcept { return glm::inverse(GetTransform()->GetMatrix()); }
		const glm::dmat4 &GetProjMatrix() const noexcept { return m_projMatrix; }
		glm::dmat4 GetViewProjMatrix() const noexcept { return m_projMatrix * glm::inverse(GetTransform()->GetMatrix()); }
		const Material* GetMaterial() const noexcept { return m_material.get(); }
	};

	class PerspectiveCamera : public Camera {
		double m_fov;
		double m_zNear;
		double m_zFar;

		void UpdateProjection() override;

	public:
		virtual Component* Clone(Actor* actor) const;

		void SetFOV(double value) noexcept {
			m_fov = value;
			UpdateProjection();
		}

		double GetFOV() const noexcept { return m_fov; }

		void SetZNear(double value) noexcept {
			m_zNear = value;
			UpdateProjection();
		}

		double GetZNear() const noexcept { return m_zNear; }

		void SetZFar(double value) noexcept {
			m_zFar = value;
			UpdateProjection();
		}

		double GetZFar() const noexcept { return m_zFar; }

		PerspectiveCamera(Actor* actor, const glm::ivec2 &frameSize, double fov, double zNear, double zFar);
	};
}
