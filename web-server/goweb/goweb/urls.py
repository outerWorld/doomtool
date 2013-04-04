from django.conf.urls import patterns, include, url
from goweb.view import html
from goweb.view import css
from goweb.view import images
from goweb.view import home

# Uncomment the next two lines to enable the admin:
# from django.contrib import admin
# admin.autodiscover()

urlpatterns = patterns(
	'',
    # Examples:
	(r'^$', home),
	(r'^/$', home),
	(r'^/index.html$', home),
	(r'.html', html),
	(r'.css$', css),
	(r'.png', images),
    # url(r'^$', 'goweb.views.home', name='home'),
    # url(r'^goweb/', include('goweb.foo.urls')),

    # Uncomment the admin/doc line below to enable admin documentation:
    # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    # url(r'^admin/', include(admin.site.urls)),
)
