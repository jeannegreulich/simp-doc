.. _DNS_Configuration:

Configure DNS
-------------

DNS must be setup and working before attempting to install clients.

If an external DNS server (not managed by SIMP) is being used 
review only the Client DNS Configuration. 

This section is not a complete manual for named. For more complete documentation
on how to set up ``named``, see named(8) and named.conf(5).


Client DNS Configuration
^^^^^^^^^^^^^^^^^^^^^^^^
By default, the SIMP module `pupmod-simp-resolv` uses the following 
hiera variables to configure DNS resolution on clients.

`simp_options::dns::servers`  An array of the IP Addresses of your DNS servers


`simp_options::dns::search` An array of strings to include in the DNS search string.

These two variables were set in  hiera by `simp config` in 
`simp_config_settings.yaml` file. However you can change them in
hiera at any time.

This should be sufficient for DNS resolution on the client.

DNS Server Configuration
^^^^^^^^^^^^^^^^^^^^^^^^

The following configuration steps are for a SIMP-managed DNS server setup.

By default the SIMP module `pupmod-simp-resolv` installs and configures
bind on any client whose IP address is in the `simp_options::dns::servers`
list.
(To disable this behavior set  `resolv::named_autoconf` to false in
hiera.)

In SIMP, numerous and/or large configuration files are distributed via
``rsync`` by Puppet to minimize management cost. These managed files presently
include DNS configuration files and can be found at
``/var/simp/environments/production/rsync/<OSTYPE>/<MAJORRELEASE>/bind_dns/default``.


#. Navigate to ``/var/simp/environments/production/rsync/<OSTYPE>/<MAJORRELEASE>/bind_dns/default``

#. Modify the ``named`` files to correctly reflect the environment.

   * The relevant files under ``bind_dns/default`` are as follows:

     * ``named/etc/named.conf``
     * ``named/etc/zones/your.domain``
     * ``named/var/named/forward/your.domain.db``
     * ``named/var/named/reverse/0.0.10.db``

   * Review ``named/etc/named.conf`` and update the following:

     * Update the :term:`IP` for allow-query and allow-recursion
     * Delete any unnecessary zone stanzas (i.e. forwarding) if not
       necessary
     * Substitute in the :term:`FQDN` of your domain for all occurrences of
       ``your.domain``

   * Add clients to ``named/var/named/forward/your.domain.db`` and
     ``named/var/named/reverse/0.0.10.db`` and then rename these files
     to appropriately match your environment.

#. Type ``puppet agent -t --tags named`` on the Puppet master to apply
   the changes.
#. Validate DNS and ensure the ``/etc/resolv.conf`` is updated appropriately.
#. If an ``rndc.key`` error appears when starting ``named``, see the `BIND
   Documentation`_.  Once you have resolved the issue, re-run the puppet
   command ``puppet agent -t`` on the Puppet master to apply.

.. NOTE::

   You can adjust the list of clients in your
   ``named/var/named/forward/<your.domain>.db`` and
   ``named/var/named/reverse/<your reverse domain>.db`` files at any time.
   Just remember to run ``puppet agent -t --tags named`` on the Puppet master
   to propagate these updates.


.. _BIND Documentation: https://www.isc.org/bind/
