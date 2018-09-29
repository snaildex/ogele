//
// Created by ??????? on 18.07.2018.
//

#ifndef OGELE_HELPERS_H
#define OGELE_HELPERS_H

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {

#define Entity(Type)\
Type ( const Type & ) = delete;\
const Type & operator=( const Type & ) = delete;

#define EnumFlags(Type)\
inline constexpr Type operator| ( Type l, Type r) noexcept { return static_cast< Type >(static_cast<std::underlying_type_t< Type >>(l)|static_cast<std::underlying_type_t< Type >>(r));}\
inline constexpr Type operator& ( Type l, Type r) noexcept { return static_cast< Type >(static_cast<std::underlying_type_t< Type >>(l)&static_cast<std::underlying_type_t< Type >>(r));}

    template<typename HandleType>
    class HandleBase {
        Entity(HandleBase)

    protected:
        HandleType m_handle;

        HandleBase() = default;

    public:
        inline HandleType GetHandle() const noexcept { return m_handle; }
    };

    list<fs::path> ScanFiles(const string &extension);
}

#endif //OGELE_HELPERS_H
