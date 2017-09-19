#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <process_handle.hpp>

TEST_CASE("all the tests")
{
    jm::process_handle h;
    REQUIRE_NOTHROW(h.owner_id());
    INFO("current process id: " << h.owner_id()
        << "\n native handle: " << h.native());

    SECTION("operator bool")
    {
        REQUIRE(static_cast<bool>(h));
    }

    SECTION("operator native_handle_t")
    {
        const auto native = static_cast<jm::native_handle_t>(h);
        REQUIRE(native == h.native());
    }

    REQUIRE_NOTHROW(h.reset());

    SECTION("assignment")
    {
        jm::process_handle other;
        auto native = static_cast<jm::native_handle_t>(other);

        REQUIRE(native == other.native());

        h = native;

        REQUIRE(h.owner_id() == other.owner_id());
        h = other;
        h = std::move(other);
    }

    SECTION("construction")
    {
        jm::process_handle other;
        auto native = static_cast<jm::native_handle_t>(other);
        jm::process_handle new_h(other);
        jm::process_handle new_h2(std::move(other));
        jm::process_handle new_h3(native);

        REQUIRE(native == new_h.native());
        REQUIRE(native == new_h2.native());
        REQUIRE(native == new_h3.native());
    }

}