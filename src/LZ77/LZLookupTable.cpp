#include "LZ77/LZLookupTable.hpp"
#include <algorithm>

LZLookupTable::LZLookupTable()
{
    m_minimumMatch = 3;
    m_slidingWindow = 4096;
    m_lookAheadWindow = 18;
    m_buffer.resize(m_minimumMatch);
}

LZLookupTable::LZLookupTable(atInt32 minimumMatch, atInt32 slidingWindow, atInt32 lookAheadWindow)
{
    if(minimumMatch > 0 )
        m_minimumMatch = minimumMatch;
    else
        m_minimumMatch = 3;
    if(slidingWindow > 0)
        m_slidingWindow = slidingWindow;
    else
        m_slidingWindow = 4096;
    if(lookAheadWindow > 0)
        m_lookAheadWindow = lookAheadWindow;
    else
        m_lookAheadWindow = 18;
    m_buffer.reserve(m_minimumMatch);
}

LZLookupTable::~LZLookupTable()
{}

void LZLookupTable::setLookAheadWindow(atInt32 lookAheadWindow)
{
    if(lookAheadWindow > 0)
        m_lookAheadWindow = lookAheadWindow;
    else
        m_lookAheadWindow = 18;
}

LZLengthOffset LZLookupTable::search(atUint8* curPos, const atUint8* dataBegin, const atUint8* dataEnd)
{
    LZLengthOffset loPair = {0,0};
    //Returns negative 1 for search failures since the current position is passed the size to be compressed
    if(curPos >=dataEnd)
    {
        loPair.length=-1;
        return loPair;
    }
    std::copy(curPos, curPos + m_minimumMatch, m_buffer.begin());
    int32_t currentOffset = static_cast<atInt32>(curPos - dataBegin);
    //Find code
    if(currentOffset > 0 && (dataEnd - curPos) >= m_minimumMatch)
    {
        auto elements = table.equal_range(m_buffer);
        elements.second--;
        elements.first--;
        //Iterate over keys in reverse order. C++11 guarantees that the relative order of elements is maintained for the same key
        for(auto iter = elements.second; iter != elements.first; iter--)
        {
            int32_t matchLength = m_minimumMatch;
            int32_t lookAheadBufferLength = ((dataEnd - curPos) < m_lookAheadWindow) ? static_cast<int32_t>(dataEnd - curPos) : m_lookAheadWindow;
            for(; matchLength < lookAheadBufferLength; ++matchLength)
            {
                if(*(dataBegin + iter->second + matchLength) != *(curPos + matchLength))
                    break;
            }
            //Store the longest match found so far into length_offset struct.
            //When lengths are the same the closer offset to the lookahead buffer wins
            if(loPair.length < (atUint32)matchLength)
            {
                loPair.length = matchLength;
                loPair.offset = currentOffset - iter->second;
            }
            //Found the longest match so break out of loop
            if(loPair.length == (atUint32)m_lookAheadWindow)
                break;
        }
    }
    //end find code
    //Insert code
    table.insert(std::make_pair(m_buffer, currentOffset));
    for(atUint32 i = 1; i < loPair.length; i++)
    {
        if(dataEnd - (curPos + i) < m_minimumMatch)
            break;
        std::copy(curPos + i, curPos + m_minimumMatch + i, m_buffer.begin());
        table.insert(std::make_pair(m_buffer, currentOffset + i));

    }
    //end insert code
    //Delete code
    int32_t slidingWindowOffset = std::max(0, currentOffset - m_slidingWindow);//Absolute offset
    int32_t tablesize=static_cast<int32_t>(table.size());
    for(int32_t i = 0; i < tablesize - m_slidingWindow; ++i)
    {
        std::copy(dataBegin + slidingWindowOffset + i, dataBegin + slidingWindowOffset + m_minimumMatch + i, m_buffer.begin());
        auto elements = table.equal_range(m_buffer);
        for(auto iter = elements.first; iter != elements.second; iter++)
        {
            if(slidingWindowOffset + i == iter->second)
            {
                table.erase(iter);
                //There should no occurance of the map with the same value
                break;
            }
        }
    }
    //end delete code
    return loPair;
    //break lookupTable.cpp:109 if table.size()> 4096

}
