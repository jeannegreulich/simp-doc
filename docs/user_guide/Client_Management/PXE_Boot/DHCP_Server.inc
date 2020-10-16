.. _DHCP_Server_Setup:

Configure DHCP
^^^^^^^^^^^^^^

.. NOTE::

   The ``dhcpd.conf`` file was updated in SIMP 6.2 to include logic in the
   ``pxeclients`` class that determines the appropriate boot loader file on the
   TFTP server, based on whether the client is booting in :term:`UEFI` or
   :term:`BIOS` mode.  If you have configured DHCP using an earlier version of
   SIMP and need to add UEFI support, make sure you update your ``dhcpd.conf``
   in the rsync directory, appropriately.

   MAC addresses in the following section need to be lower case letters.

Perform the following actions as ``root`` on the Puppet master system
prior to attempting to install a client.

Open the ``/var/simp/environments/production/rsync/<OSTYPE>/Global/dhcpd/dhcpd.conf``
file and edit it to suit the necessary environment.

Make sure the following is done in the ``dhcpd.conf`` :

  * The ``next-server`` setting in the ``pxeclients`` class block points to the
    IP Address of the :term:`TFTP` server.
  * Create a Subnet block and edit the following:

    - Make sure the **router** and **netmask** are correct for your
      environment.
    - Enter the hardware ethernet and fixed-address for each client that will
      be kickstarted.  For increased security, it is suggested that SIMP
      environments not allow clients to pick random IP Address in a subnet.
      The MAC address must be associated with and IP Address here. (You can add
      additional ones as needed.)
    - Enter the domain name for option **domain-name**
    - Enter the IP Address of the DNS server for option **domain-name-servers**
    - If PXE booting is being done with this DHCP server, make sure each ``filename``
      parameter corresponds to the correct boot loader file on the TFTP server.
      If you are using SIMP's ``simp::server::kickstart`` class to manage the
      TFTP server, the default ``filename`` values listed in the ``pxeclients``
      class of the sample ``dhpcd.conf`` will be correct.

Save and close the file.

Run ``puppet agent -t`` on the Puppet master to apply the changes.

