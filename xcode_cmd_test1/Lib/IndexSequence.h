#pragma once

namespace xxx
{

    template<size_t...T>
    struct IndexSequence
    {
        typedef IndexSequence<T...> type;
    };

    template<size_t...T>
    struct MakeIndexSequence;

    template<>
    struct MakeIndexSequence < 0 >
    {
        typedef IndexSequence<>::type type;
    };

    template<>
    struct MakeIndexSequence < 1 >
    {
        typedef IndexSequence<0>::type type;
    };

    template<size_t...TN>
    struct MakeIndexSequence < 0, TN... >
    {
        typedef typename IndexSequence<0, TN...>::type type;
    };

    template<size_t N>
    struct MakeIndexSequence < N >
    {
        typedef typename MakeIndexSequence<N - 2, N - 1>::type type;
    };

    template<size_t N, size_t... TN>
    struct MakeIndexSequence < N, TN... >
    {
        typedef typename MakeIndexSequence<N - 1, N, TN...>::type type;
    };



    template<typename...TS>
    struct HasParms
    {
        static const bool value = sizeof...( TS ) > 0;
    };

}
