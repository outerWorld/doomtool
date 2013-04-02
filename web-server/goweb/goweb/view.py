from django.http import HttpResponse
from django.http import HttpRequest
from django.template import Template, Context

def home(request):
	t = Template(open("goweb/pages/index.html", "r").read())
	ctx = Context()
	return HttpResponse(t.render(ctx))

def css(request):
	t = Template(open("goweb/pages/index.css").read())
	ctx = Context()
	return HttpResponse(t.render(ctx))	

def images(request):
	t = Template(open("goweb/images/bk.png").read())
	ctx = Context()
	return HttpResponse(t.render(ctx))

