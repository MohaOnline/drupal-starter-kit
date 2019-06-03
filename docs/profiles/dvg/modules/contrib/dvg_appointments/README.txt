# Introducton

DvG Appointment allows the use of webforms with appointment
backend applications.

It currently provides the ability to:

 - Create appointments in the backend system
 - Cancel appointments in the backend system

Supported systems:

- JCC G-BOS
- Qmatic suite 4

# Webform integration

## Configure a webform to create an appointment

The webform must be a multi-page webform via page breaks.

- Add the Appointment product component to a page
- Add Appointment date/time component to the next page
- Add a number of fields for data such as name, date of birth and email you need
  with the appointment
- Enable preview
- Enable 'Appointment' in the webform 'Form settings'. Map the requested data to
  the fields you've created

Next, mark this node as functional content for DvG Appointments.

Visit admin/config/content/functional-content, go to DvG Appointments and
add the node id to 'Create appointment'.

## Configure a webform to cancel an appointment

- Add the Appointment component to a page
- Add Appointment email to the same page
- Enable preview and make sure to include the 'Appointment'
  component as a preview value.

Next, mark this node as functional content for DvG Appointments.

Visit admin/config/content/functional-content, go to DvG Appointments and
add the node id to 'Cancel appointment'.

# Appointment backends

At the moment JCC G-Bos (gplan) and Qmatic Suite 4 (qmatic) are supported.
Support for JCC G-Bos comes in two variations; The adapter 'gplan_multiple'
supports multiple products in one backend appointment using just one planner
object. The other adapter 'gplan' books appointments with multiple products
as multiple backend appointments.

If your JCC GPLAN application supports multiple products per appointment, and is
configured to do so, use 'gplan_multiple'.

# Developer information

## Writing additional backends

To add additional backends, create a module implementing
hook_dvg_appointments_api_client. This hook must return an array of adapters:

return array(
  'adapter_machine_name' => array(
    'name' => t('Adapter Human ReadableName'),
    'class' => 'Implementing class',
  ),
);

The implementing class must extend the class AppointmentsClientApi.
