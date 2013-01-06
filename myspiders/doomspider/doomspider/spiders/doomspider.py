from scrapy.spider import BaseSpider
from scrapy.http import Request
from scrapy.selector import HtmlXPathSelector
import redis


class doomspider(BaseSpider):
	name = "doomspider"
	start_urls = [ 'http://www.baidu.com/' ]

	def __init__(self, *a, **kw):
		super(doomspider, self).__init__(*a, **kw)	
		# connect to url status db
		#    get the address of db from settings
		if not hasattr(self, "url_stat_db"):
			self.url_stat_db = redis.Redis(host='localhost', port=6379, db=0)
		#    get the address of db from settings
		# connect to url page storage db

		# get the rules

	# dispatch different url to parser
	def dispatcher(self, response):
		pass

	def parse(self, response):
		pass
