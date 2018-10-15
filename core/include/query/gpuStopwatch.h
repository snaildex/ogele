#pragma once

#include <memory>
#include <deque>

#include "query.h"
#include "../helpers/helpers.h"
#include "../other/enums.h"

namespace ogele {

    template<int SampleCount>
    class GPUStopwatch {
        Entity(GPUStopwatch)

    private:
        std::unique_ptr<Query> m_query;
        std::deque<double> m_samples;
        double m_time;
    public:
        GPUStopwatch() {
            m_query = std::make_unique<Query>(QueryType::TimeElapsed);
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