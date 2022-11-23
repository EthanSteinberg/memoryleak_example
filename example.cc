#include <Python.h>
#include "structmember.h" 

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <array>
#include <cstring>


struct custom_function {
    PyObject_HEAD
};

PyObject* custom_call(PyObject *self, PyObject *args, PyObject *kwargs) {
	std::cout << "Calling custom function with except, simpler" << std::endl;

	PyErr_SetString(PyExc_SystemError, "Fancier exception for custom fancy func");
	return nullptr;
};

PyTypeObject custom_function_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "custom_function",
    .tp_basicsize = sizeof(custom_function),
    .tp_itemsize = 0,
	.tp_call = custom_call,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("Custom function type"),
    .tp_new = PyType_GenericNew,
};

PyModuleDef helloworld_mod = {
	PyModuleDef_HEAD_INIT,
	"helloworld",
	PyDoc_STR("Hellow world module"),
	-1};

PyMODINIT_FUNC PyInit_helloworld(void)
{
	int ret = PyType_Ready(&custom_function_type);
	if (ret != 0) {
		throw std::runtime_error("Could not create pybind_function type");
	}

	PyObject *m = PyModule_Create(&helloworld_mod);

	{
		PyCFunction func = [](PyObject *, PyObject *) -> PyObject *
		{
			std::cout << "Calling the simple func" << std::endl;

			return PyLong_FromLong(34);
		};

		auto def = new PyMethodDef();
		std::memset(def, 0, sizeof(PyMethodDef));
		def->ml_name = "testing_no_except";
		def->ml_meth = func;
		def->ml_flags = METH_VARARGS;

		PyObject *m_ptr = PyCFunction_NewEx(def, nullptr, nullptr);
		PyModule_AddObject(m, "simple_func", m_ptr);
	}

	{

        PyCFunction func = [](PyObject *, PyObject *) -> PyObject * {
            std::cout << "Calling the fancy func" << std::endl;

            PyErr_SetString(PyExc_SystemError, "Fancy exception for fancy func");
            return nullptr;
        };

        auto def = new PyMethodDef();
        std::memset(def, 0, sizeof(PyMethodDef));
        def->ml_name = "testing_except";
        def->ml_meth = func;
        def->ml_flags = METH_VARARGS;

        PyObject *m_ptr = PyCFunction_NewEx(def, nullptr, nullptr);
        PyModule_AddObject(m, "fancy_func", m_ptr);
    }

    {
        PyObject *om = (PyObject *) PyObject_New(custom_function, &custom_function_type);

        PyModule_AddObject(m, "custom_fancy_func", om);
    }


	return m;
}
