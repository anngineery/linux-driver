import pytest
import subprocess


@pytest.fixture(autouse=True)
def setup():
    subprocess.run(["./setup.sh"])


def test_write_then_read():
    file_name = "/proc/my_file_node"
    text = "first write"
    f = open(file_name, "w")
    f.write(text)
    f.close()

    f = open(file_name, "r")
    content = f.read()
    f.close()

    assert text == content
    assert len(text) == len(content)
