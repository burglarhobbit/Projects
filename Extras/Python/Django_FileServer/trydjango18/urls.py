"""trydjango18 URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/1.9/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  url(r'^$', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  url(r'^$', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.conf.urls import url, include
    2. Add a URL to urlpatterns:  url(r'^blog/', include('blog.urls'))
"""
from django.conf import settings
from django.conf.urls import url
from django.conf.urls.static import static
from django.contrib import admin
from newsletter import views

urlpatterns = [
#    url(r'^$', views.home, name='home'),
	url(r'^(?P<urlPath>.*)$', views.view_real_time_index, name='view_real_time_index'),
    #url(r'^$', views.home, name='home'),
    url(r'^download/$', views.download_file, name='download_file'),
    url(r'^contact/$', views.contact, name='contact'),
    url(r'^hello/$', views.contact, name='contact'),
	url(r'^updateIndex/$', views.update_index, name='update_index'),
	url(r'^viewIndex/$', views.view_index, name='view_index'),
	#url(r'^realTime/$', views.view_real_time_index, name='view_real_time_index'),
	url(r'^realTime/(?P<urlPath>.*)$', views.view_real_time_index, name='view_real_time_index'),
    url(r'^admin/', admin.site.urls)
]

if settings.DEBUG:
	urlpatterns += static(settings.STATIC_URL, document_root=settings.STATIC_ROOT)
	urlpatterns += static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)