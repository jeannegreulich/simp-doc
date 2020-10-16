Extract the full OS Package Set
-----------------------------------------

The SIMP ISO attempts to contain everything that you need to run a base system.
However, if you did not install via ISO, or you require additional stock
packages, you can use the following procedure to extract the vendor ISOs.

#. Log on as ``simp`` and run ``su -`` to gain root access.
#. Run puppet for the first time.

   Type: ``puppet agent -t``

#. Copy the appropriate vendor OS ISO(s) to the server and unpack using the
   ``unpack_dvd`` utility. This creates a new tree under
   ``/var/www/yum/<OperatingSystem>`` suitable for serving to clients.

   Type: ``unpack_dvd CentOS-RHEL_MAJOR_VERSION-x86_64-DVD-####.iso``

#. Update your system using :term:`yum`. The updates applied will depend on
   what ISO you initially used.

   Type: ``yum clean all; yum makecache``

