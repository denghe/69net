#pragma once

namespace xxx
{
    template<typename T>
    void DbValue::Assign( T v )
    {
        GetColumnData().SetValue( parent->rowIndex, v );
#if __DEBUG
        if( Nullable() )
        {
#endif
            GetNullflag().SetFalse( parent->rowIndex );
#if __DEBUG
        }
#endif
    }

    
    template<typename T>
    T DbValue::GetValue() const
    {
        return GetColumnData().GetValue<T>( parent->rowIndex );
    }

    
    template<typename T>
    T& DbValue::RefValue()
    {
        return GetColumnData().RefValue<T>( parent->rowIndex );
    }
    
    template<typename T>
    void DbValue::SetValue( T&& v )
    {
        return GetColumnData().SetValue<T>( parent->rowIndex, std::forward<T>( v ) );
    }

}
