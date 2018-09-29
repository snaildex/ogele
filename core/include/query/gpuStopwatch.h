//
// Created by ??????? on 21.07.2018.
//

#ifndef OGELE_GPUSTOPWATCH_H
#define OGELE_GPUSTOPWATCH_H
namespace ogele {

    template<int SampleCount>
    class GPUStopwatch {
        Entity(GPUStopwatch)

    private:
        unique_ptr<Query> m_query;
        deque<double> m_samples;
        double m_time;
    public:
        GPUStopwatch() {
            m_query = make_unique<Query>(QueryType::TimeElapsed);
            m_time = 0;
        }

        inline double GetTime() const noexcept { return m_time; }

        inline void Start() { m_query->Begin(); }

        void Stop() {
            m_query->End();
            m_samples.push_back(m_query->GetResultU64() / 1000000000.0);
            if (m_samples.size() > SampleCount)
                m_samples.pop_front();
            m_time = 0;
            for (double t : m_samples)
                m_time += t;
            m_time /= m_samples.size();
        }
    };
}
#endif //OGELE_GPUSTOPWATCH_H
