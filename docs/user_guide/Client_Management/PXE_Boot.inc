.. _Configure_PXE_Boot:

Configure PXE Boot
==================

Sample kickstart templates have been provided in the ``/var/www/ks``
directory on the SIMP server  and on the SIMP DVD under ``/ks``. Pre-boot images are
located in the DVD under ``/images/pxeboot``.  These are valid only for the
OS provided on the DVD.  To get these files for other OSs read the
"How to setup kickstart for diferent OS versions."

If you have an existing :term:`Preboot Execution Environment` (PXE) setup you can use
these to PXE a SIMP client. Follow your own sites procedures for this.

In this section we describe how to configure the Kickstart, DHCP  and :term:`TFTP`
servers to PXE boot a SIMP client.  DNS  is required for PXE booting and is
discussed in an earlier chapter.

.. NOTE::

   This example sets up a PXE boot for a system that is the same OS as the SIMP
   Server. If you are setting up a PXE boot for a different OS then you must
   make sure that the OS packages are available for all systems you are trying
   to PXE boot through YUM. There are notes throughout the instructions to help
   in setting multiple OS but they are not comprehensive. You should
   understand DHCP, KS, YUM and TFTP relationships for PXE booting before
   attempting this.

.. include:: PXE_Boot/Kickstart_Server.inc

.. include:: PXE_Boot/DHCP_Server.inc

.. include:: PXE_Boot/TFTP_setup.inc

For more information see the `RedHat 6 PXE`_ or `RedHat 7 PXE`_ Installation Guides.

.. _RedHat 7 PXE: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/installation_guide/chap-installation-server-setup#sect-network-boot-setup
.. _RedHat 6 PXE: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/6/html/installation_guide/s1-netboot-pxe-config
