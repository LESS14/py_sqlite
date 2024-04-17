import unittest
import sqlite_api

class TestSQLite(unittest.TestCase):
    def test_connect(self):
        # Test database connection
        db_name = "test.db"
        db = sqlite_api.connect(db_name)
        self.assertIsNotNone(db)
        sqlite_api.close(db)

    def test_execute(self):
        # Test SQL query execution
        db_name = "test.db"
        db = sqlite_api.connect(db_name)
        self.assertIsNotNone(db)

        query = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT)"
        sqlite_api.execute(db, query)

        query = "INSERT INTO users (name) VALUES ('Alice')"
        sqlite_api.execute(db, query)

        query = "SELECT * FROM users"
        sqlite_api.execute(db, query)

        sqlite_api.close(db)

if __name__ == "__main__":
    unittest.main()
