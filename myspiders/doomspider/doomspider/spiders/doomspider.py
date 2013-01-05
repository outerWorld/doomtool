from scrapy.spider import BaseSpider
from scrapy.http import Request
from scrapy.selector import HtmlXPathSelector


class doomspider(BaseSpider):
	name = "doomspider"
	start_urls = [ '' ]

	def __init__(self, *a, **kw):
		super(doomspider, self).__init__(*a, **kw)	

	def parse(self, response):
		pass
