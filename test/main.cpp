#include <catch_with_main.hpp>
#include <process_handle.hpp>

TEST_CASE("all the tests")
{
    {
        jm::process_handle h;
        INFO("current process id: " << h.owner_id() << "\n native handle: " << h.native());
    }

    SECTION("operator bool")
    {
        jm::process_handle h;
        REQUIRE(static_cast<bool>(h));
    }

    SECTION("reset(std::nullptr_t)")
    {
        jm::process_handle h;
        h.reset();
    }

    SECTION("reset(native_handle_t)")
    {
        jm::process_handle h;
        jm::process_handle h2;
        h.reset(h2.native());
        REQUIRE(h.native() == h2.native());
        h.reset();
    }

    SECTION("assignment")
    {
        jm::process_handle h;
        jm::process_handle h2;

        h = h2;
        REQUIRE(h.native() == h2.native());
        REQUIRE(h.owner_id() == h2.owner_id());
        h = std::move(h2);
        REQUIRE_FALSE(static_cast<bool>(h2));
    }

    SECTION("copy construction")
    {
        std::error_code ec;
        jm::process_handle h;
        jm::process_handle new_h(h);
        jm::process_handle new_h2(h.native());
        REQUIRE(h.native() == new_h.native());
        REQUIRE(h.native() == new_h2.native());
        REQUIRE(h.owner_id() == new_h.owner_id());
        REQUIRE(h.owner_id() == new_h2.owner_id());
        REQUIRE(h.owner_id() == new_h.owner_id(ec));
        REQUIRE(h.owner_id() == new_h2.owner_id(ec));
        REQUIRE_FALSE(ec);
    }

    SECTION("move construction")
    {
        std::error_code ec;
        jm::process_handle h;
        jm::process_handle new_h(h);

        REQUIRE(h.native() == new_h.native());
        jm::process_handle new_h2(std::move(new_h));

        REQUIRE_FALSE(static_cast<bool>(new_h));
        REQUIRE(static_cast<bool>(new_h2));

        REQUIRE(h.owner_id() == new_h2.owner_id());
        REQUIRE(h.owner_id() == new_h2.owner_id(ec));
        REQUIRE_FALSE(ec);
    }

    SECTION("move construction")
    {
        std::error_code ec;
        jm::process_handle h;

        jm::process_handle new_h(h.owner_id());
        jm::process_handle new_h2(h.owner_id(), ec);

        REQUIRE(h.owner_id() == new_h2.owner_id());
        REQUIRE(h.owner_id() == new_h2.owner_id(ec));
    }

}