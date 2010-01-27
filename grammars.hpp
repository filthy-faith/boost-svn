/*=============================================================================
    Copyright (c) 2002 2004 2006 Joel de Guzman
    Copyright (c) 2004 Eric Niebler
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_QUICKBOOK_GRAMMARS_HPP)
#define BOOST_SPIRIT_QUICKBOOK_GRAMMARS_HPP

#include <boost/spirit/include/qi_core.hpp>
#include <boost/spirit/include/qi_symbols.hpp>
#include <boost/scoped_ptr.hpp>

namespace quickbook
{
    namespace qi = boost::spirit::qi;

    template <typename Iterator, typename Actions>
    struct phrase_grammar : qi::grammar<Iterator>
    {
        phrase_grammar(Actions& actions, bool& no_eols);
        ~phrase_grammar();

        struct rules;
        boost::scoped_ptr<rules> rules_pimpl;
        qi::rule<Iterator> start;
    private:
        phrase_grammar(phrase_grammar const&);
        phrase_grammar& operator=(phrase_grammar const&);
    };

    template <typename Iterator, typename Actions>
    struct simple_phrase_grammar : qi::grammar<Iterator>
    {
        simple_phrase_grammar(Actions& actions);
        ~simple_phrase_grammar();

        struct rules;
        boost::scoped_ptr<rules> rules_pimpl;
        qi::rule<Iterator> start;
    private:
        simple_phrase_grammar(simple_phrase_grammar const&);
        simple_phrase_grammar& operator=(simple_phrase_grammar const&);
    };

    template <typename Iterator, typename Actions, bool skip_initial_spaces = false>
    struct block_grammar : qi::grammar<Iterator>
    {
        block_grammar(Actions& actions);
        ~block_grammar();

        struct rules;
        boost::scoped_ptr<rules> rules_pimpl;
        qi::rule<Iterator> start;
    private:
        block_grammar(block_grammar const&);
        block_grammar& operator=(block_grammar const&);
    };

    template <typename Iterator, typename Actions>
    struct doc_info_grammar : qi::grammar<Iterator>
    {
        doc_info_grammar(Actions& actions);
        ~doc_info_grammar();

        struct rules;
        boost::scoped_ptr<rules> rules_pimpl;
        qi::rule<Iterator> start;
    private:
        doc_info_grammar(doc_info_grammar const&);
        doc_info_grammar& operator=(doc_info_grammar const&);
    };

    // TODO: Duplicate declaration:
    
    struct code_snippet_actions;

    template <typename Iterator>
    struct python_code_snippet_grammar
        : qi::grammar<Iterator>
    {
        typedef code_snippet_actions actions_type;
    
        python_code_snippet_grammar(actions_type& actions);
        ~python_code_snippet_grammar();

        struct rules;
        boost::scoped_ptr<rules> rules_pimpl;
        qi::rule<Iterator> start;
    private:
        python_code_snippet_grammar(python_code_snippet_grammar const&);
        python_code_snippet_grammar& operator=(python_code_snippet_grammar const&);
    };

    template <typename Iterator>
    struct cpp_code_snippet_grammar
        : qi::grammar<Iterator>
    {
        typedef code_snippet_actions actions_type;
    
        cpp_code_snippet_grammar(actions_type& actions);
        ~cpp_code_snippet_grammar();

        struct rules;
        boost::scoped_ptr<rules> rules_pimpl;
        qi::rule<Iterator> start;
    private:
        cpp_code_snippet_grammar(cpp_code_snippet_grammar const&);
        cpp_code_snippet_grammar& operator=(cpp_code_snippet_grammar const&);
    };
}

#endif // BOOST_SPIRIT_QUICKBOOK_GRAMMARS_HPP


