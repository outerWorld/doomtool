from django.http import HttpResponse
from django.http import HttpRequest
from django.template import Template, Context

def html(request):
	if request.path == "/":
		res = "goweb/pages/" + "index.html"
	else:
		res = "goweb/pages" + request.path
	print res
	t = Template(open(res, "r").read())
	ctx = Context()
	return HttpResponse(t.render(ctx))

def css(request):
	res = "goweb/" + request.path
	t = Template(open(res).read())
	ctx = Context()
	return HttpResponse(t.render(ctx))	

def images(request):
	res = "goweb" + request.path
	#t = Template(open(res).read())
	#ctx = Context()
	return HttpResponse(open(res).read(), content_type="image/png")
