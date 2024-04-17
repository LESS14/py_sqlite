#include <Python.h>
#include <sqlite3.h>

static PyObject* sqlite_connect(PyObject* self, PyObject* args) {
    const char* db_name;

    if (!PyArg_ParseTuple(args, "s", &db_name)) {
        return NULL;
    }

    sqlite3* db;
    int rc = sqlite3_open(db_name, &db);

    if (rc != SQLITE_OK) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to open database");
        return NULL;
    }

    return PyCapsule_New(db, "sqlite3*", [](PyObject* obj) {
        sqlite3* db = static_cast<sqlite3*>(PyCapsule_GetPointer(obj, "sqlite3*"));
        if (db) {
            sqlite3_close(db);
        }
        });
}

static PyObject* sqlite_close(PyObject* self, PyObject* args) {
    PyObject* db_capsule;

    if (!PyArg_ParseTuple(args, "O", &db_capsule)) {
        return NULL;
    }

    sqlite3* db = static_cast<sqlite3*>(PyCapsule_GetPointer(db_capsule, "sqlite3*"));
    if (!db) {
        PyErr_SetString(PyExc_RuntimeError, "Invalid database connection");
        return NULL;
    }

    sqlite3_close(db);
    Py_RETURN_NONE;
}

static PyObject* sqlite_execute(PyObject* self, PyObject* args) {
    PyObject* db_capsule;
    const char* query;

    if (!PyArg_ParseTuple(args, "Os", &db_capsule, &query)) {
        return NULL;
    }

    sqlite3* db = static_cast<sqlite3*>(PyCapsule_GetPointer(db_capsule, "sqlite3*"));
    if (!db) {
        PyErr_SetString(PyExc_RuntimeError, "Invalid database connection");
        return NULL;
    }

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to prepare statement");
        return NULL;
    }

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to execute query");
        return NULL;
    }

    PyObject* cursor = PyLong_FromVoidPtr(static_cast<void*>(stmt));
    if (!cursor) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to create cursor");
        return NULL;
    }

    return cursor;
}


static PyMethodDef SQLiteMethods[] = {
    {"connect", sqlite_connect, METH_VARARGS, "Connect to SQLite database"},
    {"close", sqlite_close, METH_VARARGS, "Close SQLite database connection"},
    {"execute", sqlite_execute, METH_VARARGS, "Execute SQLite query"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef sqlite_module = {
    PyModuleDef_HEAD_INIT,
    "sqlite_api",
    NULL,
    -1,
    SQLiteMethods
};

PyMODINIT_FUNC PyInit_sqlite_api(void) {
    return PyModule_Create(&sqlite_module);
}
