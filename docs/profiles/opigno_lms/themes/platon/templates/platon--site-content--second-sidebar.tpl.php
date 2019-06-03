<div id="second-sidebar" class="clearfix">
  <?php if (!empty($page['help'])): ?>
    <div id="help">
      <?php print render($page['help']); ?>
    </div>
  <?php endif; ?>

  <?php if (!empty($breadcrumb)): ?>
    <div id="breadcrumb">
      <?php print $breadcrumb; ?>
    </div>
  <?php endif; ?>

  <?php if (!empty($title)): ?>
    <div id="title-wrapper">
      <?php print render($title_prefix); ?>
      <h1><?php print $title; ?></h1>
      <?php print render($title_suffix); ?>

      <?php if (!empty($og_context_navigation)): ?>
        <div id="og-context-navigation">
          <?php print $og_context_navigation; ?>
        </div>
      <?php endif; ?>
    </div>
  <?php endif; ?>

  <?php if (!empty($messages)): ?>
    <div id="messages">
      <?php print render($messages); ?>
    </div>
  <?php endif; ?>

  <?php if (!(empty($tabs['#primary']) && empty($tabs['#secondary'])) && empty($hide_tabs)): ?>
    <div id="tabs">
      <?php print render($tabs); ?>
    </div>
  <?php endif; ?>

  <div class="flex-wrapper">
    <div id="content">
      <?php if ($action_links&&!isset($node)): ?>
        <ul class="action-links">
          <?php print render($action_links); ?>
        </ul>
      <?php endif; ?>
      <?php print render($page['content']); ?>
      <?php print render($page['content_bottom']); ?>
    </div>

    <?php
      if (isset($group_state)&& !empty($group_state)) {
        print render($group_state);
      }
    ?>

    <?php if ($action_links&&isset($node)): ?>
      <ul class="action-links">
        <?php print render($action_links); ?>
      </ul>
    <?php endif; ?>
  </div>

</div>
