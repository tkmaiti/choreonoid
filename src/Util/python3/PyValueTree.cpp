/*!
  @author Shin'ichiro Nakaoka
*/

#include "PyUtil.h"
#include "../ValueTree.h"

using namespace std;
namespace py = pybind11;
using namespace cnoid;

namespace {

MappingPtr ValueNode_toMapping(ValueNode& self){
    return self.toMapping();
}

ListingPtr ValueNode_toListing(ValueNode& self){
    return self.toListing();
}

py::object ValueNode_readInt(ValueNode& self){
    int value;
    if(self.read(value)){
        return py::object(py::cast(value));
    }
    return py::object();
}

py::object ValueNode_readFloat(ValueNode& self){
    double value;
    if(self.read(value)){
        return py::object(py::cast(value));
    }
    return py::object();
}

py::object ValueNode_readBool(ValueNode& self){
    bool value;
    if(self.read(value)){
        return py::object(py::cast(value));
    }
    return py::object();
}

py::object ValueNode_readString(ValueNode& self){
    string value;
    if(self.read(value)){
        return py::object(py::cast(value));
    }
    return py::object();
}

ValueNodePtr Mapping_find(Mapping& self, const std::string& key){
    return self.find(key);
}

MappingPtr Mapping_findMapping(Mapping& self, const std::string& key){
    return self.findMapping(key);
}

ListingPtr Mapping_findListing(Mapping& self, const std::string& key){
    return self.findListing(key);
}

ValueNodePtr Mapping_get(Mapping& self, const std::string& key){
    return &(self.get(key));
}

void Mapping_insert1(Mapping& self, const std::string& key, ValueNodePtr node){
    self.insert(key, node);
}

void Mapping_insert2(Mapping& self, MappingPtr other){
    self.insert(other);
}

MappingPtr Mapping_openMapping(Mapping& self, const std::string& key){
    return self.openMapping(key);
}

MappingPtr Mapping_openFlowStyleMapping(Mapping& self, const std::string& key){
    return self.openFlowStyleMapping(key);
}

MappingPtr Mapping_createMapping(Mapping& self, const std::string& key){
    return self.createMapping(key);
}

MappingPtr Mapping_createFlowStyleMapping(Mapping& self, const std::string& key){
    return self.createFlowStyleMapping(key);
}

ListingPtr Mapping_openListing(Mapping& self, const std::string& key){
    return self.openListing(key);
}

ListingPtr Mapping_openFlowStyleListing(Mapping& self, const std::string& key){
    return self.openFlowStyleListing(key);
}

ListingPtr Mapping_createListing(Mapping& self, const std::string& key){
    return self.createListing(key);
}

ListingPtr Mapping_createFlowStyleListing(Mapping& self, const std::string& key){
    return self.createFlowStyleListing(key);
}

py::object Mapping_readString(Mapping& self, const std::string& key){
    string value;
    if(self.read(key, value)){
        return py::object(py::cast(value));
    }
    return py::object();
}

py::object Mapping_readBool(Mapping& self, const std::string& key){
    bool value;
    if(self.read(key, value)){
        return py::object(py::cast(value));
    }
    return py::object();
}

py::object Mapping_readInt(Mapping& self, const std::string& key){
    int value;
    if(self.read(key, value)){
        return py::object(py::cast(value));
    }
    return py::object();
}

py::object Mapping_readFloat(Mapping& self, const std::string& key){
    double value;
    if(self.read(key, value)){
        return py::object(py::cast(value));
    }
    return py::object();
}

py::object Mapping_get2(Mapping& self, const std::string& key, py::object defaultValue){
    if(PyBool_Check(defaultValue.ptr())){
        bool value;
        if(self.read(key, value)){
            return py::object(py::cast(value));
        }
    } else if(PyLong_Check(defaultValue.ptr())){
        int value;
        if(self.read(key, value)){
            return py::object(py::cast(value));
        }
    } else if(PyFloat_Check(defaultValue.ptr())){
        double value;
        if(self.read(key, value)){
            return py::object(py::cast(value));
        }
    } else if(PyBytes_Check(defaultValue.ptr())){
        string value;
        if(self.read(key, value)){
            return py::object(py::cast(value));
        }
    } else {
        return py::object();
    }
    return defaultValue;
}

void Mapping_writeString1(Mapping& self, const std::string &key, const std::string& value){
    self.write(key, value);
}

void Mapping_writeString2(Mapping& self, const std::string &key, const std::string& value, StringStyle style){
    self.write(key, value, style);
}

void Mapping_write(Mapping& self, const std::string &key, py::object value){
    if(PyBool_Check(value.ptr())){
        self.write(key, value.cast<bool>());
    } else if(PyLong_Check(value.ptr())){
        self.write(key, value.cast<int>());
    } else if(PyFloat_Check(value.ptr())){
        self.write(key, value.cast<double>());
    } else {
        PyErr_SetString(PyExc_TypeError, "The argument type is not supported");
        throw py::error_already_set();
    }
}

ValueNodePtr Listing_front(Listing& self){
    return self.front();
}

ValueNodePtr Listing_back(Listing& self){
    return self.back();
}

ValueNodePtr Listing_at(Listing& self, int i){
    return self.at(i);
}

void Listing_write_int(Listing& self, int i, int value){
    self.write(i, value);
}

void Listing_write_string1(Listing& self, int i, const std::string& value){
    self.write(i, value);
}

void Listing_write_string2(Listing& self, int i, const std::string& value, StringStyle stringStyle){
    self.write(i, value, stringStyle);
}

MappingPtr Listing_newMapping(Listing& self){
    return self.newMapping();
}

void Listing_append_node(Listing& self, ValueNodePtr node){
    self.append(node);
}

void Listing_append_int(Listing& self, int value){
    self.append(value);
}

void Listing_append_double(Listing& self, double value){
    self.append(value);
}

void Listing_append_string1(Listing& self, const std::string& value){
    self.append(value);
}

void Listing_append_string2(Listing& self, const std::string& value, StringStyle style){
    self.append(value, style);
}


}

namespace cnoid {

void exportPyValueTree(py::module& m)
{
    py::enum_<StringStyle>(m, "StringStyle")
        .value("PLAING_STRING", PLAIN_STRING)
        .value("SINGLE_QUOTED", SINGLE_QUOTED)
        .value("DOUBLE_QUOTED", DOUBLE_QUOTED)
        .value("LITERAL_STRING", LITERAL_STRING)
        .value("FOLDED_STRING", FOLDED_STRING)
        .export_values();

    py::class_<ValueNode, ValueNodePtr, Referenced>(m, "ValueNode")
        .def("isValid", &ValueNode::isValid)
        .def("toInt", &ValueNode::toInt)
        .def("toFloat", &ValueNode::toDouble)
        .def("toBool", &ValueNode::toBool)
        .def("isScalar", &ValueNode::isScalar)
        .def("isString", &ValueNode::isString)
        .def("toString", &ValueNode::toString)
        .def("isMapping", &ValueNode::isMapping)
        .def("toMapping", ValueNode_toMapping)
        .def("isListing", &ValueNode::isListing)
        .def("toListing", ValueNode_toListing)
        .def("readInt", ValueNode_readInt)
        .def("readFloat", ValueNode_readFloat)
        .def("readBool", ValueNode_readBool)
        .def("readString", ValueNode_readString)
        .def("hasLineInfo", &ValueNode::hasLineInfo)
        .def("line", &ValueNode::line)
        .def("column", &ValueNode::column)
        ;

    py::class_< Mapping, MappingPtr, ValueNode>(m, "Mapping")
        .def("empty", &Mapping::empty)
        .def("size", &Mapping::size)
        .def("clear", &Mapping::clear)
        .def("setFlowStyle", &Mapping::setFlowStyle)
        .def("isFlowStyle", &Mapping::isFlowStyle)
        .def("setFloatFormat", &Mapping::setDoubleFormat)
        .def("floatFormat", &Mapping::doubleFormat)
        .def("setKeyQuoteStyle", &Mapping::setKeyQuoteStyle)
        .def("find", Mapping_find)
        .def("findMapping", Mapping_findMapping)
        .def("findListing", Mapping_findListing)
        .def("get", Mapping_get)
        .def("__getitem__", Mapping_get)
        .def("insert", Mapping_insert1)
        .def("insert", Mapping_insert2)
        .def("openMapping", Mapping_openMapping)
        .def("openFlowStyleMapping", Mapping_openFlowStyleMapping)
        .def("createMapping", Mapping_createMapping)
        .def("createFlowStyleMapping", Mapping_createFlowStyleMapping)
        .def("openListing", Mapping_openListing)
        .def("openFlowStyleListing", Mapping_openFlowStyleListing)
        .def("createListing", Mapping_createListing)
        .def("createFlowStyleListing", Mapping_createFlowStyleListing)
        .def("remove", &Mapping::remove)
        .def("readString", Mapping_readString)
        .def("readBool", Mapping_readBool)
        .def("readInt", Mapping_readInt)
        .def("readFloat", Mapping_readFloat)
        .def("get", Mapping_get2)
        .def("write", Mapping_writeString1)
        .def("write", Mapping_writeString2)
        .def("write", Mapping_write)
        ;

    py::class_< Listing, ListingPtr, ValueNode>(m, "Listing")
        .def("empty", &Listing::empty)
        .def("size", &Listing::size)
        .def("clear", &Listing::clear)
        .def("reserve", &Listing::reserve)
        .def("setFlowStyle", &Listing::setFlowStyle)
        .def("isFlowStyle", &Listing::isFlowStyle)
        .def("setFloatFormat", &Listing::setDoubleFormat)
        .def("floatFormat", &Listing::doubleFormat)
        .def("front", Listing_front)
        .def("back", Listing_back)
        .def("at", Listing_at)
        .def("write", Listing_write_int)
        .def("write", Listing_write_string1)
        .def("write", Listing_write_string2)
        .def("__getitem__", Listing_at)
        .def("newMapping", Listing_newMapping)
        .def("append", Listing_append_node)
        .def("append", Listing_append_int)
        .def("append", Listing_append_double)
        .def("append", Listing_append_string1)
        .def("append", Listing_append_string2)
        .def("appendLF", &Listing::appendLF)
        ;

}

}