Adding or Updating SIMP Packages to a deployed environemnt.
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. Warning::

  Before Upgrading/Updating a :ref:`Local deployment
  scenario<ug-sa-env-deployment-scenarios--local>` make sure you have put any local
  modules you are maintaining  in the environment directory in the Puppetfile
  or they will be erased.

The following instructions are specific to the :ref:`Local deployment
scenario<ug-sa-env-deployment-scenarios--local>` and to SIMP version 6.4
and later.

The section steps through adding or updating individual modules in a deployed
eenvironemnt.  SIMP delivers extra modules that are not installed by default and user may want to add some or all of these to a
deployemnt.  This section assumes the updated RPMs have allready been added to the yum
repositiories on your system.

This example will step you through adding a simp-extras module, pupmod-simp-simp_nfs,
and a patch pupmod-simp-fips release 2.0.1 (currently installed is release 2.0.0).

The example assumes the Puppet environment you are updating is named ``test``,
and that you execute these steps as ``root``:


#. Update the YUM Repositories

   * Follow your site's procedures to update your repositories.

#. Install the RPMs

   .. code-block:: sh

      # Apply updates to the local master
      # Install the new rpm
      yum install pupmod-simp-simp_nfs
      # Update the existing rpm
      yum update pupmod-simp-fips

   This will update the system-local, SIMP-managed
   Puppet module :term:`Git` repositories.  It will not remove any old tags from
   Puppet module :term:`Git` repositories.  You can still revert back to the older
   version or point to different versions in different environments.

#. Follow the generic instructions for updating a local environment. (Copied
   here for your convenience.)

  .. NOTE::

     Using  ``simp puppetfile generate > Puppetfile.simp`` will always use the
     latest tags from the local git repositories.  If this is not wanted you must
     hand edit the file.  The article :ref:`How To Generate a SIMP Puppetfile<howto-generate-a-simp-puppetfile>` for
     basic information on Puppetfile syntax.

.. include:: ../common/Update_and_Deploy_Local_Environment.inc

At this point the test environment should be updated and any systems in this
environment will be able to use the simp_nfs module and will also use the new
updated fips module the next time puppet is run.

