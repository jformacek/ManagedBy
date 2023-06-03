# ManagedBy
Implements exception in membership onlocal Administrators group, based on security principal reference contained in managedBy attribute of computer account in AD.

Idea is pretty simple:
- AD admin puts reference to user or security group to managedBy attribute of computer account
- this solution works as GPO Client Side Extension and every time GPO is refreshed, it looks into managedBy attribute and adds user or group present there to local Administrators group
  - this addition happens after built-in Windows GPOs run and setup membership to standard defined by GPO's
- so ManagedBy solution effectively implements exception mechanism for Administartors group membership on per-computer basis

Solution comes with own ADMX template that turns the logic on/off.
