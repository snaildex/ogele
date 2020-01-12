#pragma once

class Submarine : public ogele::Component {
public:
	struct Stats {
		double Speed;
		double Turn;
	};
private:
	Stats m_stats;
	double m_curSpeed;
public:
	Submarine(ogele::Actor* actor);
	void Update() override;
	void OnGui() override;
	std::string GetName() const override { return "Submarine controller"; }
	const Stats& GetStats() const noexcept { return m_stats; }
	double GetCurrentSpeed() const noexcept { return m_curSpeed; }
};