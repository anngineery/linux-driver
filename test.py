import pytest


@pytest.fixture()
def setup():
    # load the module get the file handler
    pass

def test_write_then_read():
    file_name = "/proc/my_file_node"
    text = "first write"
    f = open(file_name, "w")
    f.write(text)
    f.close()

    f = open(file_name, "r")
    assert text == f.read()
    f.close()





