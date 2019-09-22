//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/vinniefalco/json
//

#ifndef BOOST_JSON_ITERATOR_HPP
#define BOOST_JSON_ITERATOR_HPP

#include <boost/json/detail/config.hpp>
#include <boost/json/value.hpp>
#include <boost/json/detail/stack.hpp>
#include <boost/json/detail/static_const.hpp>
#include <iterator>
#include <new>

namespace boost {
namespace json {

struct end_t
{
};

BOOST_JSON_INLINE_VARIABLE(end, end_t)

/** A generator to perform a depth-first traversal of a JSON value.
*/
class const_iterator
{
#ifndef GENERATING_DOCUMENTATION
    struct node
    {
        // The XSLT has trouble with private anonymous unions
        union
        {
            array::const_iterator arr_it;
            object::const_iterator obj_it;
        };
        value const* v;
        string_view  key;
        bool         it;

        BOOST_JSON_DECL
        ~node();

        BOOST_JSON_DECL
        node(
            value const& v_,
            bool it_) noexcept;

        BOOST_JSON_DECL
        node(node const& other) noexcept;

        BOOST_JSON_DECL
        bool
        last() const noexcept;
    };
#endif

    detail::stack<node, 50> stack_;

public:
    struct value_type
    {
        std::size_t         depth;
        string_view         key;
        json::value const&  value;
        bool                last;
        bool                end;

        value_type const*
        operator->() const noexcept
        {
            return this;
        }
    };

    BOOST_JSON_DECL
    explicit
    const_iterator(
        value const& jv);

    BOOST_JSON_DECL
    value_type
    operator*() const noexcept;

    BOOST_JSON_DECL
    const_iterator&
    operator++() noexcept;

    void
    operator++(int) noexcept
    {
        ++*this;
    }

    value_type
    operator->() const noexcept
    {
        return *(*this);
    }

    friend
    bool
    operator==(
        const_iterator const& lhs,
        end_t) noexcept
    {
        return lhs.stack_.empty();
    }

    friend
    bool
    operator==(
        end_t,
        const_iterator const& rhs) noexcept
    {
        return rhs.stack_.empty();
    }

    friend
    bool
    operator!=(
        const_iterator const& lhs,
        end_t) noexcept
    {
        return ! lhs.stack_.empty();
    }

    friend
    bool
    operator!=(
        end_t,
        const_iterator const& rhs) noexcept
    {
        return ! rhs.stack_.empty();
    }
};

} // json
} // boost

#if BOOST_JSON_HEADER_ONLY
#include <boost/json/impl/iterator.ipp>
#endif

#endif
