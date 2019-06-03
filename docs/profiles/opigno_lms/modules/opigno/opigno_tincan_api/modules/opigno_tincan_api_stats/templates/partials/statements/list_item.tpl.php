<tr>
  <td><?php print $statement->actor->name; ?></td>
  <td><?php print $statement->verb->display->{'en-US'}; ?></td>
  <td><?php print $statement->object->definition->name->{'en-US'}; ?></td>
  <td><?php print $statement->timestamp->format('Y-m-d H:i:s'); ?></td>
</tr>