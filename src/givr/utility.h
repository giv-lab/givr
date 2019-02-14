#include <tuple>
#include <type_traits>
#include <utility>

namespace givr {
namespace utility {

    // named types
    template <typename T, typename Parameter> class Type {
        public:
            explicit Type() = default;
            explicit Type(T const &value) : m_value(value) {}

            template <typename... Args>
            explicit Type(Args &&... args) : m_value(std::forward<Args>(args)...) {}

            T &value() { return m_value; }
            T const &value() const { return m_value; }
            T &operator*() noexcept { return m_value; }
            operator T() const { return m_value;}

        private:
            T m_value;
    };

    template <typename T, typename Parameter>
    std::ostream &operator<<(std::ostream &out, Type<T, Parameter> const &type) {
        return out << type.value();
    }

    // contains
    template <typename T, typename... Ts>
    constexpr bool contains = (std::is_same<T, Ts>{} || ...);

    // is subset of
    template <typename Subset, typename Set> constexpr bool is_subset_of = false;

    template <typename... Ts, typename... Us>
    constexpr bool is_subset_of<std::tuple<Ts...>, std::tuple<Us...>> =
        (contains<Ts, Us...> && ...);

    // has duplicates
    template <typename... T1> constexpr bool has_duplicate_types = false;

    template <typename T1, typename T2>
    constexpr bool has_duplicate_types<T1, T2> = (std::is_same<T1, T2>::value);

    template <typename T1, typename T2, typename... Trest>
    constexpr bool has_duplicate_types<T1, T2, Trest...> =
        (has_duplicate_types<T1, T2> ||       // test each other
         has_duplicate_types<T1, Trest...> || // test T1 vs. rest
     has_duplicate_types<T2, Trest...>);  // test T2 vs. rest

    template <typename... Types> struct TypedStruct {
        using Args = std::tuple<Types...>;
        Args args;

        template <typename T> T const &value() const { return std::get<T>(args); }
        template <typename T> T &value() { return std::get<T>(args); }

        template <typename T> void set(T &&v) { value<T>() = std::forward<T>(v); }
        void set(Args const &a) { args = a; }

        template <typename T, typename... Args> void set(T &&v, Args &&... args) {
            value<T>() = std::forward<T>(v);
            set(std::forward<Args>(args)...);
        }
    };

} // namespace utility
} // namespace givr
