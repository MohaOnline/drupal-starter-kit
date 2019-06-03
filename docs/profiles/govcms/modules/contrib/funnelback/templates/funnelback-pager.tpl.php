<?php
/**
 * @file
 * Template file for the funnelback pager.
 *
 * Available variables:
 * - $summary: An array of result summary information
 */
?>
<?php if (count($pager['pages']) > 1): ?>
  <ul class="funnelback-pager">

    <?php if (isset($pager['first_link'])): ?>
      <li class="pager-first"><a href="<?php print $pager['first_link'] ?>">First</a></li>
    <?php endif ?>

    <?php if (isset($pager['first']) && !$pager['first']): ?>
      <li class="pager-prev"><a href="<?php print $pager['prev_link'] ?>">Prev</a></li>
    <?php endif ?>

    <?php foreach($pager['pages'] as $page): ?>

      <?php if ($page['current']): ?>
        <li class="pager-current"><?php print $page['title'] ?></li>
      <?php else: ?>
        <li class="pager-item"><a href="<?php print $page['link'] ?>"><?php print $page['title'] ?></a></li>
      <?php endif; ?>

    <?php endforeach ?>

    <?php if (isset($pager['last']) && !$pager['last']): ?>
      <li class="pager-next"><a href="<?php print $pager['next_link'] ?>">Next</a></li>
    <?php endif ?>

    <?php if (isset($pager['last_link'])): ?>
      <li class="pager-last"><a href="<?php print $pager['last_link'] ?>">Last</a> </li>
    <?php endif ?>

  </ul>
<?php endif ?>