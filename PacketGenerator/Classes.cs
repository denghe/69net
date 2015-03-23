using System;
using System.Collections.Generic;

public class Template
{
    public List<Project> Projects = new List<Project>();
    public List<Class> Classes = new List<Class>();
    public List<Enum> Enums = new List<Enum>();
    public List<String> Namespaces = new List<string>();
}

public class Project
{
    public string Name = "";
    public string Desc = "";
    // public bool Enabled = true;
}

public class ClassBase
{
    public string Namespace = "";
    public string Name = "";
    public string Desc = "";
    public bool IsEnum = true;
    //public List<Project> Enable = new List<Project>();    // 这个并不直接来源于属性，一但被某个属于某个 __project 的类引用到，则须生成到该 project
}

public class Class : ClassBase
{
    public List<ClassField> Fields = new List<ClassField>();
    public ushort TypeID = 0;                               // 该值将自增填充
    //public List<Project> Decode = new List<Project>();    // 被引用到的类 即便没有直接设置 Attribute，也会继承引用类的设置
    //public List<Project> Encode = new List<Project>();    // 同上
}

public class ClassField
{
    public Class Class = null;
    public string Name = "";
    public Declare Declare = new Declare();
    public string Desc = "";

    public bool Get = false;
    public bool Set = false;
    public object Default = true;

    public Dictionary<ClassField, object> Condation = new Dictionary<ClassField,object>();

    //public bool Enable
    //public decimal MinValue = 0;
    //public decimal MaxValue = 0;
    //public DecodeCondation DecodeCondation = new DecodeCondation( "", Operators.Equal, null );
    //public EncodeCondation EncodeCondation = new EncodeCondation( "", Operators.Equal, null );
}

public class Enum : ClassBase
{
    public List<EnumField> Fields = new List<EnumField>();
    public string Type = "";
    public int Size = 0;        // numBytes
    public bool Unsigned = false;
    // ...
}

public class EnumField
{
    public Enum Enum = null;
    public string Name = "";
    public string Value = "";
    public string Desc = "";
}

public enum DataTypes
{
    BuiltIn,    // 内置 各种整数浮点
    Custom,     // 用户类，枚举
    Generic,    // 泛型
    Array       // 数组
}
public class Declare
{
    public DataTypes DataType;
    public string Name;
    public string Namespace;
    public ClassBase Class;     // 只针对 用户类，枚举
    public int MinLen = 0;
    public int MaxLen = 0;
    public List<Declare> Childs = new List<Declare>();  // 只针对 数组 或 泛型
}
