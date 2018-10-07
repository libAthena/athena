#pragma once

#include "YAMLCommon.hpp"

namespace athena::io
{

class YAMLDocWriter
{
    std::unique_ptr<YAMLNode> m_rootNode;
    std::vector<YAMLNode*> m_subStack;
    yaml_emitter_t m_emitter;
    static bool RecursiveFinish(yaml_emitter_t* doc, const YAMLNode& node);
    void _leaveSubRecord();
    void _leaveSubVector();
public:
    YAMLDocWriter(const char* classType, athena::io::IStreamReader* reader = nullptr);
    ~YAMLDocWriter();

    yaml_emitter_t* getEmitter() { return &m_emitter; }

    bool finish(athena::io::IStreamWriter* fout);

    inline YAMLNode* getCurNode() const { return m_subStack.empty() ? nullptr : m_subStack.back(); }

    class RecordRAII
    {
        friend class YAMLDocWriter;
        YAMLDocWriter* m_w = nullptr;
        RecordRAII(YAMLDocWriter* w) : m_w(w) {}
        RecordRAII() = default;
    public:
        operator bool() const { return m_w != nullptr; }
        ~RecordRAII() { if (m_w) m_w->_leaveSubRecord(); }
    };
    friend class RecordRAII;

    RecordRAII enterSubRecord(const char* name);

    template <class T>
    void enumerate(const char* name, T& record,
                   typename std::enable_if_t<__IsDNARecord_v<T>>* = 0)
    {
        if (auto rec = enterSubRecord(name))
            record.write(*this);
    }

    class VectorRAII
    {
        friend class YAMLDocWriter;
        YAMLDocWriter* m_w = nullptr;
        VectorRAII(YAMLDocWriter* w) : m_w(w) {}
        VectorRAII() = default;
    public:
        operator bool() const { return m_w != nullptr; }
        ~VectorRAII() { if (m_w) m_w->_leaveSubVector(); }
    };
    friend class VectorRAII;

    VectorRAII enterSubVector(const char* name);

    template <class T>
    void enumerate(const char* name, const std::vector<T>& vector,
                   typename std::enable_if_t<!std::is_arithmetic<T>::value &&
                                             !std::is_same<T, atVec2f>::value &&
                                             !std::is_same<T, atVec3f>::value &&
                                             !std::is_same<T, atVec4f>::value &&
                                             !std::is_same<T, atVec2d>::value &&
                                             !std::is_same<T, atVec3d>::value &&
                                             !std::is_same<T, atVec4d>::value>* = 0)
    {
        if (auto v = enterSubVector(name))
            for (const T& item : vector)
                if (auto rec = enterSubRecord(nullptr))
                    item.write(*this);
    }

    template <class T>
    void enumerate(const char* name, const std::vector<T>& vector,
                   typename std::enable_if_t<std::is_arithmetic<T>::value ||
                                             std::is_same<T, atVec2f>::value ||
                                             std::is_same<T, atVec3f>::value ||
                                             std::is_same<T, atVec4f>::value ||
                                             std::is_same<T, atVec2d>::value ||
                                             std::is_same<T, atVec3d>::value ||
                                             std::is_same<T, atVec4d>::value>* = 0)
    {
        if (auto v = enterSubVector(name))
            for (T item : vector)
                writeVal<T>(nullptr, item);
    }

    template <class T>
    void enumerate(const char* name, const std::vector<T>& vector,
                   std::function<void(YAMLDocWriter&, const T&)> writef)
    {
        if (auto v = enterSubVector(name))
            for (const T& item : vector)
                if (auto rec = enterSubRecord(nullptr))
                    writef(*this, item);
    }

    template <typename INTYPE>
    void writeVal(const char* name, const INTYPE& val);
    template <typename INTYPE>
    void writeVal(const char* name, const INTYPE& val, size_t byteCount);
    void writeBool(const char* name, const bool& val);
    void writeByte(const char* name, const atInt8& val);
    void writeUByte(const char* name, const atUint8& val);
    void writeInt16(const char* name, const atInt16& val);
    void writeUint16(const char* name, const atUint16& val);
    void writeInt32(const char* name, const atInt32& val);
    void writeUint32(const char* name, const atUint32& val);
    void writeInt64(const char* name, const atInt64& val);
    void writeUint64(const char* name, const atUint64& val);
    void writeFloat(const char* name, const float& val);
    void writeDouble(const char* name, const double& val);
    void writeVec2f(const char* name, const atVec2f& val);
    void writeVec3f(const char* name, const atVec3f& val);
    void writeVec4f(const char* name, const atVec4f& val);
    void writeVec2d(const char* name, const atVec2d& val);
    void writeVec3d(const char* name, const atVec3d& val);
    void writeVec4d(const char* name, const atVec4d& val);
    void writeUBytes(const char* name, const std::unique_ptr<atUint8[]>& val, size_t byteCount);
    void writeString(const char* name, std::string_view val);
    void writeWString(const char* name, std::wstring_view val);
    void writeU16String(const char* name, std::u16string_view val);
    void writeU32String(const char* name, std::u32string_view val);

    void setStyle(YAMLNodeStyle s);
};

}

