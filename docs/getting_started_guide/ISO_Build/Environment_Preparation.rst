.. _gsg-environment_preparation:

Environment Preparation
=======================

A non-root user should be used to build the simp iso.  The preparation
is divided into two parts.  The first, System Configuration, must be done
by a user with sudo privileges.  The second, User Configuration, must
be done any user who is going to build the iso on the system.

System Configuration
--------------------

The following should be performed by a user with root privileges.

Ensure Sufficient Entropy
^^^^^^^^^^^^^^^^^^^^^^^^^

The SIMP build generates various keys and does quite a bit of package
signing. As such, your system must be able to keep its entropy pool
full at all times. If you check ``/proc/sys/kernel/random/entropy_avail``
and it shows a number below **1024**, then you should either make sure that
``rngd`` is running and pointed to a hardware source (preferred) or install
and use **haveged**.

.. code-block:: bash

  $ sudo yum install haveged
  $ sudo systemctl start haveged

RVM Manager
^^^^^^^^^^^

The rvm manager is set up by the user. However, if the user does
not have sudo privileges the following packages must be installed prior to them
setting up their rvm manager:

.. code-block:: bash

  $ sudo yum install -y patch autoconf automake bison bzip2 gcc-c++ libffi-devel libtool patch \
    readline-devel sqlite-devel zlib-devel glibc-headers glibc-devel libyaml-devel openssl-devel


Other Tools
^^^^^^^^^^^

You will need git installed on your system"

.. code-block:: bash

  $ sudo yum install git


If you are running the build in docker it will build the environment.  If you are running build:auto
then the following rpms need to be installed:

.. code-block:: bash

  $ sudo yum install rpm-build rubygems selinux-policy-devel checkpolicy yum-utils policycoreutils-python \
                rpmdevtools libicu-devel libxml2 libxml2-devel libxslt libxslt-devel  \
                rpm-sign gcc gcc-c++ ruby-devel

Set Up Docker
^^^^^^^^^^^^^

Docker is typically provided by an OS repository.  You may need to enable that
repository depending on your distribution.

.. code-block:: bash

  $ sudo yum install docker

Allow your (non-root) user to run docker:

.. code-block:: bash

  $ sudo usermod -aG dockerroot <user>

.. NOTE::

  You may need to log out and log back in before your user is able to run as
  dockerroot.

Edit ``/etc/docker/daemon.json`` and change the ownership of the docker daemon
socket:

.. code-block:: json

  {
    "live-restore": true,
    "group": "dockerroot"
  }

By default docker limits systems to 20G.  The ISO build requires more space.
Edit the /etc/sysconfig/docker-storage file to add the following options (or
read docker-storage-setup to determine how to configure your storage):

.. code-clock:: bash

DOCKER_STORAGE_OPTIONS=--storage-opt dm.basesize=100G

Start the docker daemon:

.. code-block:: bash

  $ sudo systmectl start docker
  $ sudo systemctl enable docker



User Configuration:
------------------
The following must be done by the user who will be building SIMP.

.. WARNING::

  Please use a not-root user for installing simp or building the iso.

Set Up Ruby
^^^^^^^^^^^

We highly recommend using :term:`RVM` to make it easy to develop and test
against several versions of :term:`Ruby` at once without damaging your
underlying Operating System.

RVM Installation
^^^^^^^^^^^^^^^^
The RVM installation should be preformed by the user who is going to
install/build simp.

The following commands, taken from the `RVM Installation Page`_ can be used to
install :term:`RVM` for your user.

.. code-block:: bash

   $ gpg2 --keyserver hkp://keys.gnupg.net --recv-keys \
       409B6B1796C275462A1703113804BB82D39DC0E3
   $ \curl -sSL https://get.rvm.io | bash -s stable --ruby=2.1.9
   $ source ~/.rvm/scripts/rvm

.. NOTE::

  The user must have sudo privileges or the following packages must lready be installed:
  yum install -y patch autoconf automake bison bzip2 gcc-c++ libffi-devel libtool patch
  readline-devel sqlite-devel zlib-devel glibc-headers glibc-devel libyaml-devel openssl-devel

Set the Default Ruby
^^^^^^^^^^^^^^^^^^^^

You'll want to use :term:`Ruby` 2.1.9 as your default :term:`RVM` for SIMP
development.

.. code-block:: bash

   $ rvm use --default 2.1.9

.. NOTE::

   Once this is done, you can simply type ``rvm use 2.1.9``.

Bundler
^^^^^^^
The next important tool is `Bundler`_. Bundler makes it easy to install Gems
and their dependencies. It gets this information from the Gemfile found in the
root of each repo. The Gemfile contains all of the gems required for working
with the repo. More info on Bundler can be found on the
`Bundler Rationale Page`_ and more information on Rubygems can be found at
`Rubygems.org`_.

.. code-block:: bash

   $ rvm all do gem install bundler


.. _Bundler Rationale Page: http://bundler.io/rationale.html
.. _Bundler: http://bundler.io/
.. _RVM Installation Page: https://rvm.io/rvm/install
.. _RVM: https://rvm.io/
.. _Rubygems.org: http://guides.rubygems.org/what-is-a-gem/
