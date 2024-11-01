Salesforce Integration
======================

This module suite implements a mapping functionality between Salesforce
objects and Backdrop entities. In other words, for each of your supported Backdrop
entities (e.g. node, user, or entities supported by extensions), you can
assign Salesforce objects that will be created / updated when the entity is
saved. For each such assignment, you choose which Backdrop and Salesforce fields
should be mapped to one another.

This suite also includes an API architecture which allows for additional
modules to be easily plugged in (e.g. for webforms, contact form submits,
etc).

For a more detailed description of each component module, see below.

Requirements
------------

1) You need a Salesforce account. Developers can register here:
   https://developer.salesforce.com/signup

2) You will need to create a remote application/connected app for
   authorization. In Salesforce go to Your Name > Setup > Create > Apps then
   create a new Connected App. Set the callback URL to:
   https://<your site>/salesforce/oauth_callback  (must use SSL)

Select at least 'Perform requests on your behalf at any time' for OAuth Scope
as well as the appropriate other scopes for your application.

Additional information:
https://help.salesforce.com/help/doc/en/remoteaccess_about.htm

3) Your site needs to be SSL enabled to authorize the remote application using
   OAUTH.

4) If using the SOAP API, PHP to have been compiled with SOAP web services and
   OpenSSL support, as per:

http://php.net/soap
http://php.net/openssl

5) Required modules
   Libraries, only for SOAP API - https://backdropcms.org/project/libraries
   Entity Plus & Entity UI, for Salesforce Mapping - https://backdropcms.org/project/entity_plus,
   https://backdropcms.org/project/entity_ui,

Modules
-------

Salesforce (salesforce):
  OAUTH2 authorization and wrapper around the Salesforce REST API.

Salesforce Mapping (salesforce_mapping)
  Map Backdrop entities to Salesforce fields, including field level mapping.

Salesforce Push (salesforce_push):
  Push Backdrop entity updates into Salesforce.

Salesforce Pull (salesforce_pull):
  Pull Salesforce object updates into Backdrop.

Salesforce Soap (salesforce_soap):
  Lightweight wrapper around the SOAP API, using the OAUTH access token, to
  fill in functional gaps missing in the REST API. Requires the Salesforce PHP
  Toolkit.

NOTES:

Addressfield Options
  Salesforce provides a formatting plugin for addressfield which converts the
  "Thoroughfare" field to a text area. Enabling this option will make it much
  easier to sync addressfield data to Salesforce multi-line addressfields.

Dates
  For Backdrop date fields with start and end dates, salesforce_pull will fail
  unless you are using a version of Date that includes commit 7faeea3.

    To avoid potential timezone conversion errors, try setting your date field
    to "No timezone conversion".

Installation
------------

- Install this module using the official Backdrop CMS instructions at
  https://docs.backdropcms.org/documentation/extend-with-modules.

- Visit the configuration page under Administration > Configuration > Salesforce
(admin/config/salesforce/authorize) and enter the required information.

- Install any of the other modules if needed to pull from and push data to
Salesforce.

- If your site is under version control, consider adding `config/staging/salesforce.auth_settings.json` to your `.gitignore` file to ensure sensitive credentials are not committed to the repository.

Issues
------

Bugs and feature requests should be reported in [the Issue Queue](https://github.com/backdrop-contrib/salesforce/issues).

Current Maintainers
-------------------

- [Eli Lisseck](https://github.com/elisseck).
- [Anthony Nemirovsky](https://github.com/anemirovsky).

Credits
-------

- Backdrop development supported by [USENIX](https://www.usenix.org/).
- Backdrop development supported by [Giant Rabbit](https://giantrabbit.com).
- Originally written for Drupal by [Steve McKenzie](http://drupal.org/user/45890).
- Ported to Backdrop CMS by [Alejandro Madrigal](https://github.com/alemadlei) & [Eli Lisseck](https://github.com/elisseck).

License
-------

This project is GPL v2 software.
See the LICENSE.txt file in this directory for complete text.
