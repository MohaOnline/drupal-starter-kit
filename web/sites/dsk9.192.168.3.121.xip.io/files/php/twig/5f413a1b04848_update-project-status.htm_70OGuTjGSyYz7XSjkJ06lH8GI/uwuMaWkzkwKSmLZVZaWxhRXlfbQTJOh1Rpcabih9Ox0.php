<?php

use Twig\Environment;
use Twig\Error\LoaderError;
use Twig\Error\RuntimeError;
use Twig\Extension\SandboxExtension;
use Twig\Markup;
use Twig\Sandbox\SecurityError;
use Twig\Sandbox\SecurityNotAllowedTagError;
use Twig\Sandbox\SecurityNotAllowedFilterError;
use Twig\Sandbox\SecurityNotAllowedFunctionError;
use Twig\Source;
use Twig\Template;

/* core/modules/update/templates/update-project-status.html.twig */
class __TwigTemplate_c25c04e11636e4224d5998fd1be22d42c55a11de2a416ded1ff7503b54031b5f extends \Twig\Template
{
    private $source;
    private $macros = [];

    public function __construct(Environment $env)
    {
        parent::__construct($env);

        $this->source = $this->getSourceContext();

        $this->parent = false;

        $this->blocks = [
        ];
        $this->sandbox = $this->env->getExtension('\Twig\Extension\SandboxExtension');
        $tags = array("set" => 31, "if" => 40, "for" => 63, "trans" => 90);
        $filters = array("escape" => 39, "join" => 85, "t" => 87, "placeholder" => 91);
        $functions = array("constant" => 32);

        try {
            $this->sandbox->checkSecurity(
                ['set', 'if', 'for', 'trans'],
                ['escape', 'join', 't', 'placeholder'],
                ['constant']
            );
        } catch (SecurityError $e) {
            $e->setSourceContext($this->source);

            if ($e instanceof SecurityNotAllowedTagError && isset($tags[$e->getTagName()])) {
                $e->setTemplateLine($tags[$e->getTagName()]);
            } elseif ($e instanceof SecurityNotAllowedFilterError && isset($filters[$e->getFilterName()])) {
                $e->setTemplateLine($filters[$e->getFilterName()]);
            } elseif ($e instanceof SecurityNotAllowedFunctionError && isset($functions[$e->getFunctionName()])) {
                $e->setTemplateLine($functions[$e->getFunctionName()]);
            }

            throw $e;
        }

    }

    protected function doDisplay(array $context, array $blocks = [])
    {
        $macros = $this->macros;
        // line 31
        $context["status_classes"] = [0 => (((twig_get_attribute($this->env, $this->source,         // line 32
($context["project"] ?? null), "status", [], "any", false, false, true, 32) == twig_constant("Drupal\\update\\UpdateManagerInterface::NOT_SECURE"))) ? ("project-update__status--security-error") : ("")), 1 => (((twig_get_attribute($this->env, $this->source,         // line 33
($context["project"] ?? null), "status", [], "any", false, false, true, 33) == twig_constant("Drupal\\update\\UpdateManagerInterface::REVOKED"))) ? ("project-update__status--revoked") : ("")), 2 => (((twig_get_attribute($this->env, $this->source,         // line 34
($context["project"] ?? null), "status", [], "any", false, false, true, 34) == twig_constant("Drupal\\update\\UpdateManagerInterface::NOT_SUPPORTED"))) ? ("project-update__status--not-supported") : ("")), 3 => (((twig_get_attribute($this->env, $this->source,         // line 35
($context["project"] ?? null), "status", [], "any", false, false, true, 35) == twig_constant("Drupal\\update\\UpdateManagerInterface::NOT_CURRENT"))) ? ("project-update__status--not-current") : ("")), 4 => (((twig_get_attribute($this->env, $this->source,         // line 36
($context["project"] ?? null), "status", [], "any", false, false, true, 36) == twig_constant("Drupal\\update\\UpdateManagerInterface::CURRENT"))) ? ("project-update__status--current") : (""))];
        // line 39
        echo "<div";
        echo $this->extensions['Drupal\Core\Template\TwigExtension']->escapeFilter($this->env, $this->sandbox->ensureToStringAllowed(twig_get_attribute($this->env, $this->source, twig_get_attribute($this->env, $this->source, ($context["status"] ?? null), "attributes", [], "any", false, false, true, 39), "addClass", [0 => "project-update__status", 1 => ($context["status_classes"] ?? null)], "method", false, false, true, 39), 39, $this->source), "html", null, true);
        echo ">";
        // line 40
        if (twig_get_attribute($this->env, $this->source, ($context["status"] ?? null), "label", [], "any", false, false, true, 40)) {
            // line 41
            echo "<span>";
            echo $this->extensions['Drupal\Core\Template\TwigExtension']->escapeFilter($this->env, $this->sandbox->ensureToStringAllowed(twig_get_attribute($this->env, $this->source, ($context["status"] ?? null), "label", [], "any", false, false, true, 41), 41, $this->source), "html", null, true);
            echo "</span>";
        } else {
            // line 43
            echo $this->extensions['Drupal\Core\Template\TwigExtension']->escapeFilter($this->env, $this->sandbox->ensureToStringAllowed(twig_get_attribute($this->env, $this->source, ($context["status"] ?? null), "reason", [], "any", false, false, true, 43), 43, $this->source), "html", null, true);
        }
        // line 45
        echo "  <span class=\"project-update__status-icon\">
    ";
        // line 46
        echo $this->extensions['Drupal\Core\Template\TwigExtension']->escapeFilter($this->env, $this->sandbox->ensureToStringAllowed(twig_get_attribute($this->env, $this->source, ($context["status"] ?? null), "icon", [], "any", false, false, true, 46), 46, $this->source), "html", null, true);
        echo "
  </span>
</div>

<div class=\"project-update__title\">";
        // line 51
        if (($context["url"] ?? null)) {
            // line 52
            echo "<a href=\"";
            echo $this->extensions['Drupal\Core\Template\TwigExtension']->escapeFilter($this->env, $this->sandbox->ensureToStringAllowed(($context["url"] ?? null), 52, $this->source), "html", null, true);
            echo "\">";
            echo $this->extensions['Drupal\Core\Template\TwigExtension']->escapeFilter($this->env, $this->sandbox->ensureToStringAllowed(($context["title"] ?? null), 52, $this->source), "html", null, true);
            echo "</a>";
        } else {
            // line 54
            echo $this->extensions['Drupal\Core\Template\TwigExtension']->escapeFilter($this->env, $this->sandbox->ensureToStringAllowed(($context["title"] ?? null), 54, $this->source), "html", null, true);
        }
        // line 56
        echo "  ";
        echo $this->extensions['Drupal\Core\Template\TwigExtension']->escapeFilter($this->env, $this->sandbox->ensureToStringAllowed(($context["existing_version"] ?? null), 56, $this->source), "html", null, true);
        echo "
  ";
        // line 57
        if (((($context["install_type"] ?? null) == "dev") && ($context["datestamp"] ?? null))) {
            // line 58
            echo "    <span class=\"project-update__version-date\">(";
            echo $this->extensions['Drupal\Core\Template\TwigExtension']->escapeFilter($this->env, $this->sandbox->ensureToStringAllowed(($context["datestamp"] ?? null), 58, $this->source), "html", null, true);
            echo ")</span>
  ";
        }
        // line 60
        echo "</div>

";
        // line 62
        if (($context["versions"] ?? null)) {
            // line 63
            echo "  ";
            $context['_parent'] = $context;
            $context['_seq'] = twig_ensure_traversable(($context["versions"] ?? null));
            foreach ($context['_seq'] as $context["_key"] => $context["version"]) {
                // line 64
                echo "    ";
                echo $this->extensions['Drupal\Core\Template\TwigExtension']->escapeFilter($this->env, $this->sandbox->ensureToStringAllowed($context["version"], 64, $this->source), "html", null, true);
                echo "
  ";
            }
            $_parent = $context['_parent'];
            unset($context['_seq'], $context['_iterated'], $context['_key'], $context['version'], $context['_parent'], $context['loop']);
            $context = array_intersect_key($context, $_parent) + $_parent;
        }
        // line 67
        echo "
";
        // line 69
        $context["extra_classes"] = [0 => (((twig_get_attribute($this->env, $this->source,         // line 70
($context["project"] ?? null), "status", [], "any", false, false, true, 70) == twig_constant("Drupal\\update\\UpdateManagerInterface::NOT_SECURE"))) ? ("project-not-secure") : ("")), 1 => (((twig_get_attribute($this->env, $this->source,         // line 71
($context["project"] ?? null), "status", [], "any", false, false, true, 71) == twig_constant("Drupal\\update\\UpdateManagerInterface::REVOKED"))) ? ("project-revoked") : ("")), 2 => (((twig_get_attribute($this->env, $this->source,         // line 72
($context["project"] ?? null), "status", [], "any", false, false, true, 72) == twig_constant("Drupal\\update\\UpdateManagerInterface::NOT_SUPPORTED"))) ? ("project-not-supported") : (""))];
        // line 75
        echo "<div class=\"project-updates__details\">
  ";
        // line 76
        if (($context["extras"] ?? null)) {
            // line 77
            echo "    <div class=\"extra\">
      ";
            // line 78
            $context['_parent'] = $context;
            $context['_seq'] = twig_ensure_traversable(($context["extras"] ?? null));
            foreach ($context['_seq'] as $context["_key"] => $context["extra"]) {
                // line 79
                echo "        <div";
                echo $this->extensions['Drupal\Core\Template\TwigExtension']->escapeFilter($this->env, $this->sandbox->ensureToStringAllowed(twig_get_attribute($this->env, $this->source, twig_get_attribute($this->env, $this->source, $context["extra"], "attributes", [], "any", false, false, true, 79), "addClass", [0 => ($context["extra_classes"] ?? null)], "method", false, false, true, 79), 79, $this->source), "html", null, true);
                echo ">
          ";
                // line 80
                echo $this->extensions['Drupal\Core\Template\TwigExtension']->escapeFilter($this->env, $this->sandbox->ensureToStringAllowed(twig_get_attribute($this->env, $this->source, $context["extra"], "label", [], "any", false, false, true, 80), 80, $this->source), "html", null, true);
                echo ": ";
                echo $this->extensions['Drupal\Core\Template\TwigExtension']->escapeFilter($this->env, $this->sandbox->ensureToStringAllowed(twig_get_attribute($this->env, $this->source, $context["extra"], "data", [], "any", false, false, true, 80), 80, $this->source), "html", null, true);
                echo "
        </div>
      ";
            }
            $_parent = $context['_parent'];
            unset($context['_seq'], $context['_iterated'], $context['_key'], $context['extra'], $context['_parent'], $context['loop']);
            $context = array_intersect_key($context, $_parent) + $_parent;
            // line 83
            echo "    </div>
  ";
        }
        // line 85
        echo "  ";
        $context["includes"] = twig_join_filter($this->sandbox->ensureToStringAllowed(($context["includes"] ?? null), 85, $this->source), ", ");
        // line 86
        echo "  ";
        if (($context["disabled"] ?? null)) {
            // line 87
            echo "    ";
            echo $this->extensions['Drupal\Core\Template\TwigExtension']->renderVar(t("Includes:"));
            echo "
    <ul>
      <li>
        ";
            // line 90
            echo t("Enabled: %includes", array("%includes" =>             // line 91
($context["includes"] ?? null), ));
            // line 93
            echo "      </li>
      <li>
        ";
            // line 95
            $context["disabled"] = twig_join_filter($this->sandbox->ensureToStringAllowed(($context["disabled"] ?? null), 95, $this->source), ", ");
            // line 96
            echo "        ";
            echo t("Disabled: %disabled", array("%disabled" =>             // line 97
($context["disabled"] ?? null), ));
            // line 99
            echo "      </li>
    </ul>
  ";
        } else {
            // line 102
            echo "    ";
            echo t("Includes: %includes", array("%includes" =>             // line 103
($context["includes"] ?? null), ));
            // line 105
            echo "  ";
        }
        // line 106
        echo "</div>
";
    }

    public function getTemplateName()
    {
        return "core/modules/update/templates/update-project-status.html.twig";
    }

    public function isTraitable()
    {
        return false;
    }

    public function getDebugInfo()
    {
        return array (  217 => 106,  214 => 105,  212 => 103,  210 => 102,  205 => 99,  203 => 97,  201 => 96,  199 => 95,  195 => 93,  193 => 91,  192 => 90,  185 => 87,  182 => 86,  179 => 85,  175 => 83,  164 => 80,  159 => 79,  155 => 78,  152 => 77,  150 => 76,  147 => 75,  145 => 72,  144 => 71,  143 => 70,  142 => 69,  139 => 67,  129 => 64,  124 => 63,  122 => 62,  118 => 60,  112 => 58,  110 => 57,  105 => 56,  102 => 54,  95 => 52,  93 => 51,  86 => 46,  83 => 45,  80 => 43,  75 => 41,  73 => 40,  69 => 39,  67 => 36,  66 => 35,  65 => 34,  64 => 33,  63 => 32,  62 => 31,);
    }

    public function getSourceContext()
    {
        return new Source("", "core/modules/update/templates/update-project-status.html.twig", "/content/drupal-starter-kit-9/web/core/modules/update/templates/update-project-status.html.twig");
    }
}
