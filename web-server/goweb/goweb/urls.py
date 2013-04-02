from django.conf.urls import patterns, include, url
from goweb.view import home
from goweb.view import css
from goweb.view import images

# Uncomment the next two lines to enable the admin:
# from django.contrib import admin
# admin.autodiscover()

urlpatterns = patterns(
	'',
    # Examples:
	(r'^$', home),
	(r'.css$', css),
	(r'^/images', images),
    # url(r'^$', 'goweb.views.home', name='home'),
    # url(r'^goweb/', include('goweb.foo.urls')),

    # Uncomment the admin/doc line below to enable admin documentation:
    # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    # url(r'^admin/', include(admin.site.urls)),
)
