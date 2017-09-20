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

    SECTION("reset")
    {
        REQUIRE_NOTHROW(h.reset());
        jm::process_handle handle;
        handle.reset(h.native());
        REQUIRE(handle.native() == h.native());
    }

    SECTION("assignment")
    {
        jm::process_handle other;
        auto native = other.native();

        REQUIRE(native == other.native());

        h = native;

        REQUIRE(h.owner_id() == other.owner_id());
        h = other;
        h = std::move(other);
    }

    SECTION("construction")
    {
        jm::process_handle other;
        auto native = other.native();
        jm::process_handle new_h(other);
        jm::process_handle new_h2(std::move(other));
        jm::process_handle new_h3(native);

        REQUIRE(native == new_h.native());
        REQUIRE(native == new_h2.native());
        REQUIRE(native == new_h3.native());
    }

}