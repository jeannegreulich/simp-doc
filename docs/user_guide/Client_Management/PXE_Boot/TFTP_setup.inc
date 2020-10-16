.. _Configure_TFTP:

Setting up TFTP
---------------

This section describes the process of setting up static files and manifests for
:term:`TFTP`.  The example in this section assumes we are configuring the
``production`` :term:`SIMP Omni-Environment`.

.. NOTE::

   The tftp root directory was changed in SIMP 6.2 to conform to DISA STIG
   standards. In previous versions it was ``/tftpboot``, and in 6.2 and later
   it is ``/var/lib/tftpboot``. If you are upgrading to 6.2 from a prior
   release and wish the files to remain in the ``/tftpboot`` directory, set
   ``tftpboot::tftpboot_root_dir`` to ``/tftpboot`` in :term:`Hiera`.

Static Files
^^^^^^^^^^^^

Verify the static files are in the correct location:

Type ``cd /var/simp/environments/production/rsync/<OSTYPE>/Global/tftpboot``

(<OSTYPE> and <MAJORRELEASE> under rsync are the type and version of the SIMP **server**.)

Verify there is a ``linux-install`` directory and cd to this directory.

Under the linux-install directory you should find a directory named
``OSTYPE-MAJORRELEASE.MINORRELEASE-ARCH`` and a **relative**-link to this
directory named ``OSTYPE-MAJORRELEASE-ARCH``.

Under OSTYPE-MAJORRELEASE.MINORRELEASE-ARCH you should find the files:

* ``initrd.img``
* ``vmlinuz``

If these files are not where they should be, then create the directories as
needed and copy the files from
``/var/www/yum/<OSTYPE>/<MAJORRELEASE>/<ARCH>/images/pxeboot``
or from the images directory on the SIMP DVD. The link name is what is used in
the resources in the tftpboot.pp manifest examples.

.. NOTE::

   The images in the tftp directory need to match the distribution. For example,
   if you upgrade your repo from CentOS 7.3 to 7.4 and will be using this repo
   to kickstart machines, you must also upgrade the images in the tftp directory.
   If they do not match you can get an error such as "unknown file system type 'xfs'"

Next you need to set up the boot files for either BIOS boot mode, UEFI mode, or both.

.. NOTE::

   UEFI support was automated in SIMP 6.2. If you are using an older version of
   SIMP please refer to that documentation for setting up UEFI manually.

For more information see the `RedHat 7 Installation Source`_  or `RedHat 6 Installation Source`_
Installation Guides.

Dynamic Linux Model Files
^^^^^^^^^^^^^^^^^^^^^^^^^
Create a :term:`site profile` module for the TFTP server on the Puppet master
to set up the various files to model different systems.

#. Create the file ``tftpboot.pp`` in your :term:`site profile`.
   This file will contain Linux models for different types of systems and
   a mapping of MAC addresses to each model.

   Use the source code example below. Linux model examples are given for
   CentOS 6 and 7 using both UEFI and BIOS boot mode.

   * Replace ``KSSERVER`` with the IP address of kickstart server (or the code
     to look up the IP Address using :term:`Hiera`).

   * Replace ``OSTYPE``, ``MAJORRELEASE`` and ``ARCH`` with the correct values
     for the systems you will be PXE booting.

   * ``MODEL NAME`` is usually of the form ``OSTYPE-MAJORRELEASE-ARCH`` for
     consistency.

   * You will need to know what kickstart file you are using. UEFI and BIOS
     mode require separate kickstart files. Other things that might require a
     different kickstart file to be configured are disk drive configurations and
     FIPS configuration. Create a different Linux model file for each different
     kickstart file needed.

     .. NOTE::
        If using the default cfg files, know that they do not have the '_el[6,7]'
        tags at the end of their names.

     .. NOTE::
        The ``simp_disk_crypt`` option shown below switches on transparent disk
        encryption as described in the :ref:`ig-disk-encryption` documentation
        and is recommended if you have a requirement for disk encryption and
        cannot enter a password at system boot time.

        Simply omit the option if you do not wish to use the capability.

     .. code-block:: ruby

        class site::tftpboot {
          include '::tftpboot'

          #--------
          # BIOS MODE MODEL EXAMPLES

          # for CentOS/RedHat 7 Legacy/BIOS boot
          tftpboot::linux_model { 'el7_x86_64':
            kernel => 'OSTYPE-MAJORRELEASE-ARCH/vmlinuz',
            initrd => 'OSTYPE-MAJORRELEASE-ARCH/initrd.img',
            ks     => "https://KSSERVER/ks/pupclient_x86_64_el7.cfg",
            extra  => "inst.noverifyssl ksdevice=bootif simp_disk_crypt\nipappend 2"
          }

          # For CentOS/RedHat 6 Legacy/BIOS boot
          # Note the difference in the `extra` arguments here.
          tftpboot::linux_model { 'el6_x86_64':
            kernel => 'OSTYPE-MAJORRELEASE-ARCH/vmlinuz',
            initrd => 'OSTYPE-MAJORRELEASE-ARCH/initrd.img',
            ks     => "https://KSSERVER/ks/pupclient_x86_64_el6.cfg",
            extra  => "noverifyssl ksdevice=bootif simp_disk_crypt\nipappend 2"
          }

          #------
          # UEFI MODE MODEL EXAMPLES

          # NOTE UEFI boot uses the linux_model_efi module and has different
          # `extra` arguments. You also would use a different kickstart file
          # because the bootloader command within the kickstart file is
          # different. Read the instructions in the default pupclient_x86_64.cfg
          # file and make sure you have the correct bootloader line.
          #
          # For CentOS/RedHat 7 UEFI boot
          tftpboot::linux_model_efi { 'el7_x86_64_efi':
            kernel => 'OSTYPE-MAJORRELEASE-ARCH/vmlinuz',
            initrd => 'OSTYPE-MAJORRELEASE-ARCH/initrd.img',
            ks     => "https://KSSERVER/ks/pupclient_x86_64_efi_el7.cfg",
            extra  => "inst.noverifyssl simp_disk_crypt"
          }

          # For CentOS/RedHat 6 UEFI boot
          # Note the extra attribute legacy_grub.
          tftpboot::linux_model_efi { 'el6_x86_64_efi':
            kernel      => 'OSTYPE-MAJORRELEASE-ARCH/vmlinuz',
            initrd      => 'OSTYPE-MAJORRELEASE-ARCH/initrd.img',
            ks          => "https://KSSERVER/ks/pupclient_x86_64_el6.cfg",
            extra       => "noverifyssl simp_disk_crypt",
            legacy_grub => true
          }

          #------
          # DEFAULT HOST BOOT CONFIGURATION EXAMPLES

          # If desired, create defaults boot configuration for BIOS and UEFI.
          # Note that the name of the default UEFI configuration file needs
          # to be 'grub.cfg'.
          tftpboot::assign_host { 'default': model => 'el7_x86_64' }
          tftpboot::assign_host_efi { 'grub.cfg': model => 'el7_x86_64_efi' }


          #------
          # HOST BOOT CONFIGURATION ASSIGNMENT EXAMPLES

          # For each system define what module you want to use by pointing
          # its MAC address to the appropriate model. Note that the MAC
          # address is preceded by ``01-``.
          tftpboot::assign_host { '01-aa-ab-ac-1d-05-11': model => 'el7_x86_64' }
          tftpboot::assign_host_efi { '01-aa-bb-cc-dd-00-11': model => 'el7_x86_64_efi' }
        }


#. Add the ``site::tftpboot`` class on your puppet server node via
   :term:`Hiera`. Create the file (or edit if it exists):
   ``/etc/puppetlabs/code/environments/production/data/hosts/<tftp.server.fqdn>.yaml``.
   (By default, the TFTP server is the same as your puppet server so it should
   exist.) Add the following example code to that yaml file.

   .. code-block:: yaml

      ---
      simp::classes:
        - 'site::tftpboot'


#. After updating the above file, type ``puppet agent -t --tags tftpboot`` on
   the Puppet master.

   .. NOTE::
      To provide PXE boot configuration for more OSs, create, in the
      ``tftpboot.pp`` file, a ``tftpboot::linux_model`` or
      ``tftpboot::linux_model_efi`` block for each OS type. Then, assign
      individual hosts to each model by adding ``tftpboot::assign_host`` or
      ``tftpboot::assign_host_efi`` resources.

.. _RedHat 7 Installation Source: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/installation_guide/sect-making-media-additional-sources#sect-making-media-sources-network
.. _RedHat 6 Installation Source: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/6/html/installation_guide/ch-Preparing-x86#s1-steps-network-installs-x86
