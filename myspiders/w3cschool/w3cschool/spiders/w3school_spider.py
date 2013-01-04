# coding: UTF-8
from scrapy.spider import BaseSpider
from scrapy.http import Request
from scrapy.selector import HtmlXPathSelector
from w3cschool.url_parser import url_parser
import sys
import os

class w3school_spider(BaseSpider):
	name = "w3school_spider"
	start_urls = [ "http://www.w3school.com.cn/" ]
	allowed_domains = [ "www.w3school.com.cn" ]
	host = "www.w3school.com.cn"
	main_url = "www.w3school.com.cn/"
	site_name = "w3school.com.cn"
	root_path = ""

	def __init__(self, *a, **kw):
		super(w3school_spider, *a, **kw)
		#dispatcher.connect(self.spider_closed, signals.spider_closed)
		if "SCRAPY_SITES_PATH" not in os.environ:
			self.root_path = os.getcwd()	
		else:
			self.root_path = os.environ["SCRAPY_SITES_PATH"]
		self.root_path = self.root_path + "/" + self.site_name
		#print "root_path = %s" %(self.root_path)
		if not os.path.exists(self.root_path):
			os.mkdir(self.root_path)
		else:
			pass

	def main_url_parser(self, url_obj, response):
		print "main_url_parser: %s" %(response.url)
		# store the page
		open(self.root_path + "/" + "index.html", "w").write(response.body)
		# fetch the sub pages from this main page.
		hxs = HtmlXPathSelector(response)
		item_list = hxs.select(r'//div[@id="navsecond"]/ul/li/a')	
		for i in range(0, len(item_list)):
			#print "item [%s] [%s]" %("".join(item_list[i].select(r'@href').extract()), "".join(item_list[i].select(r'text()').extract()))
			req_url = self.host + "".join(item_list[i].select(r'@href').extract())
			print "Request url = %s" %(req_url)
			yield Request(req_url, 'GET')

	def sub_url_parser(self, url_obj, response):
		print "sub_url_parser: %s" %(response.url)
		pass

	def dispatch_parse(self, url_obj, response):
		uri_path = url_obj.get_uri_path()
		store_path = self.root_path + uri_path
		print "%s" %(store_path)
		if not os.path.exists(store_path):
			os.mkdir(store_path)
		else:
			pass
		
		print "uri_path = %s" %(uri_path)
		if uri_path == "/":
			self.main_url_parser(url_obj, response)
		else:
			self.sub_url_parser(url_obj, response)

	def parse(self, response):
		url_obj = url_parser(response.url)
		url_obj.show()
		#print "%s, %s" %(os.getcwd(), self.root_path)
		self.dispatch_parse(url_obj, response)
