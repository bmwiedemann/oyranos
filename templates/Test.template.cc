#include "oyTest.h"

{% for class in classes %}
#include "{{ class.name }}.h"
#include "{{ class.privName }}.h"
{% endfor %}

{% for class in classes %}
void oyTest::{{ class.name }}_generic_methods()
{
  {{ class.name }} *{{ class.baseName|lower }} = NULL;
  {{ class.baseName|lower }} = oy{{ class.baseName }}_New(0);

  QVERIFY( {{ class.baseName|lower }} != NULL );

  {{ class.name }} *{{ class.baseName|lower }}_copy = NULL;
  {{ class.baseName|lower }}_copy = oy{{ class.baseName }}_Copy({{ class.baseName|lower }}, 0);

  QVERIFY( {{ class.baseName|lower }} == {{ class.baseName|lower }}_copy );

  oyObject_s obj = oyObject_New();
  {{ class.name }} *{{ class.baseName|lower }}_new = NULL;
  {{ class.baseName|lower }}_new = oy{{ class.baseName }}_Copy({{ class.baseName|lower }}, obj);

  QVERIFY( {{ class.baseName|lower }} != {{ class.baseName|lower }}_new );

  oy{{ name.baseName }}_Release( &{{ class.baseName|lower }} );
  oy{{ name.baseName }}_Release( &{{ class.baseName|lower }}_copy );
  oy{{ name.baseName }}_Release( &{{ class.baseName|lower }}_new );
}

void oyTest::{{ class.privName }}_generic_methods()
{

}
{% endfor %}
