import subprocess
import os
from contextlib import contextmanager


@contextmanager
def cd(newdir):
    prevdir = os.getcwd()
    os.chdir(os.path.expanduser(newdir))
    try:
        yield
    finally:
        os.chdir(prevdir)


def patch_and_deploy(url):
    url = url.rstrip()
    if "#" in url:
        repo = url.split("#")[0]
        rev = url.split("#")[1]
        folder = url.split("/")[-2]
    else:
        repo = url
        if not url.endswith("/"):
            url = url + "/"
        folder = url.split("/")[-2]
    repo = repo.replace("gitlab.com", "gitlab-ci-token:${CI_JOB_TOKEN}@gitlab.com")

    if not os.path.exists(folder):
        try:
            subprocess.check_call(["git clone " + repo], shell=True)
        except subprocess.CalledProcessError:
            subprocess.check_call(["hg clone " + repo], shell=True)

    with cd(folder):
        if "#" in url:
            try:
                subprocess.call(["git checkout " + rev], shell=True)
            except subprocess.CalledProcessError:
                subprocess.call(["hg update " + rev], shell=True)
        else:
            try:
                subprocess.call(["git checkout master"], shell=True)
            except subprocess.CalledProcessError:
                subprocess.call(["hg update default"], shell=True)
        deploy_libs()


def deploy_libs():
    """Deploy libraries in current directory"""
    for lib in os.listdir(os.getcwd()):
        if lib.endswith(".lib"):
            with open(os.getcwd() + "/" + lib) as file:
                url = file.read()
                patch_and_deploy(url)


def main():
    deploy_libs()


if __name__ == "__main__":
    main()
