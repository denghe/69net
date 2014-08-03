using System;
using System.Collections.Generic;

public class Template
{
    //public List<Project> Projects = new List<Project>();
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
    public bool IsEnum = true;
    //public List<Project> Enable = new List<Project>();    // 这个并不直接来源于属性，一但被某个属于某个 __project 的类引用到，则须生成到该 project
}

public class Class : ClassBase
{
    public List<ClassField> Fields = new List<ClassField>();
    public string Desc = "";
    public ushort TypeID = 0;                               // 该值将自增填充
    //public List<Project> Decode = new List<Project>();    // 被引用到的类 即便没有直接设置 Attribute，也会继承引用类的设置
    //public List<Project> Encode = new List<Project>();    // 同上
}

public class ClassField
{
    public Class Class = null;
    public string Name = "";
    public string TypeNamespace = "";
    public string Type = "";
    public ClassBase TypeClass = null;
    public Container Container = null;

    //public bool Nullable = false;
    //public bool IsArray = false;
    //public bool IsList = false;
    //public bool IsDictionary = false;
    //public string KeyTypeNamespace = "";        // IsDictionary 时有意义
    //public string KeyType = "";                 // IsDictionary 时有意义
    //public ClassBase KeyTypeClass = null;       // IsDictionary 时有意义
    // ...
    public string Desc = "";

    public bool Get = false;
    public bool Set = false;
    public object Default = true;

    public uint MinLen = 0;
    public uint MaxLen = 0;
    public Dictionary<ClassField, object> Condation = new Dictionary<ClassField,object>();

    //public decimal MinValue = 0;
    //public decimal MaxValue = 0;
    //public DecodeCondation DecodeCondation = new DecodeCondation( "", Operators.Equal, null );
    //public EncodeCondation EncodeCondation = new EncodeCondation( "", Operators.Equal, null );
}

public enum ContainerTypes
{
    Array,
    List,
    Dict
}
public class Container
{
    public ContainerTypes Type;
    public Container ParentContainer;

    public string KeyType = "";

    public string ValueTypeNamespace = "";
    public string ValueType = "";
    public ClassBase ValueTypeClass = null;
    public Container ValueContainer = null;
}

public class Enum : ClassBase
{
    public List<EnumField> Fields = new List<EnumField>();
    public string Type = "";
    public int Size = 0;        // numBytes
    public bool Unsigned = false;
    // ...
    public string Desc = "";
}

public class EnumField
{
    public Enum Enum = null;
    public string Name = "";
    public string Value = "";
    public string Desc = "";
}
