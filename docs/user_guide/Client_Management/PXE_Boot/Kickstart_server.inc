.. _Setting_Up_Kickstart:

Setting up Kickstart
--------------------

This section describes how to configure the kickstart server.

#. Add the Kickstart Server Profile

   - In the Puppet server-specific :term:`Hiera` file (by default located at
     ``/etc/puppetlabs/code/environments/production/data/hosts/puppet.<your.domain>.yaml``),
     add the ``simp::server::kickstart`` class.

     .. code-block:: yaml

        ---
        simp::classes:
          - simp::server::kickstart

   - This profile class adds management of DHCP, TFTP and the Kickstart service,
     as well as the example provisioning script.

   - After adding the above class, run puppet: ``puppet agent -t``.

#. Locate the following files in the ``/var/www/ks`` directory

   - ``pupclient_x86_64.cfg``: Example client kickstart configuration script.
   - ``diskdetect.sh``:  Example script to determine disks available
     on a system and then apply disk configuration. This script is used
     by ``pupclient_x86_64.cfg``.

#. Open the ``pupclient_x86_64.cfg`` file and follow the instructions provided
   within it to replace the variables listed and to customize for
   :term:`BIOS`/:term:`UEFI` boot and/or FIPS/non-FIPS mode. If you have servers
   that require different boot mode or FIPS options, you will need to make
   customized copies of this file to provide those distinct configurations. You
   will also have to configure TFTP to point to the appropriate files.

   - Instructions are provided both at the top of the file and throughout the
     body of the file.
   - You need to know the IP Addresses of the YUM, Kickstart, and TFTP servers.
     (They default to the SIMP server in ``simp config``).
   - Use the commands described in the comments at the top of the file to
     generate the root and GRUB passwords hashes. Be sure to replace ``password``
     with your root password. The GRUB password can also be managed later with
     the ``simp-simp_grub`` module. See
     :ref:`Managing GRUB Users <grub_user_management>` for more information.
   - Follow the instructions throughout the file to customize for BIOS/UEFI boot.
   - Follow the instructions throughout the file to customize for FIPS/non-FIPS
     mode.

#. Open the ``diskdetect.sh`` script and customize the disk device names and/or
   partitions as appropriate for your site. The sample ``diskdetect.sh`` script
   will work, as is, for most systems, as long as your disk device names are in the
   list. In addition, the sample script provides STIG-compliant partitioning.

#. Type ``chown root.apache /var/www/ks/*`` to ensure that all files are owned
   by ``root`` and in the ``apache`` group.

#. Type ``chmod 640 /var/www/ks/*`` to change the permissions so the owner can
   read and write the file and the ``apache`` group can only read.

.. NOTE::

   Two major changes were made to ``pupclient_x86_64.cfg`` in SIMP 6.2:

   - UEFI PXE support was added.
   - To address timeout issues that caused Puppet bootstrap failures, the use of
     the ``runpuppet`` script to bootstrap Puppet on the client was replaced
     with the use of two scripts, both provided by the ``simp::server::kickstart``
     class:

     - A ``systemd`` unit file for CentOS 7 (``simp_client_bootstrap.service``)
       or a ``systemv`` init script for CentOS 6 (``simp_client_bootstrap``).
     - A common bootstrap script (``bootstrap_simp_client``) used by both.

.. NOTE::

   The URLs and locations in the file are set up for a default SIMP install.
   That means the same OS and version as the SIMP server, all servers in one
   location (on the SIMP server) and in specific directories. If you have
   installed these servers in a different location than the defaults, you may
   need to edit URLs or directories.

.. NOTE::

   If you want to PXE boot more than this operating system, make a copy of
   these files, name them appropriately and update URLS and links inside and
   anything else you may need. (You must know what you are doing before
   attempting this.) If you are booting more than one OS you must also make
   sure your YUM server has the OS packages for the other OSs. By default, the
   YUM server on SIMP has the packages only for the version of OS installed on
   the SIMP server.
