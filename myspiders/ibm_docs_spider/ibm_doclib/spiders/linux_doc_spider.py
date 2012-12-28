from scrapy.spider import BaseSpider
from scrapy.selector import HtmlXPathSelector
from scrapy.http import Request
import time

class linux_doc_spider(BaseSpider):
	name = "linux_doc_spider"
	start_urls = [ "http://www.ibm.com/developerworks/cn/views/linux/libraryview.jsp"]
					#"http://www.ibm.com/developerworks/cn/views/linux/libraryview.jsp?site_id=10&contentarea_by=Linux&sort_by=&sort_order=2&start=101&end=200&topic_by=&product_by=&type_by=%E6%89%80%E6%9C%89%E7%B1%BB%E5%88%AB&show_abstract=true&search_by=&industry_by=&series_title_by=",
					#"http://www.ibm.com/developerworks/cn/linux/l-cn-awk-httplog/index.html" ]
	res_name = ""
	listpage_chara = ["developerworks", "cn", "views", "linux"]
	contentpage_chara = ["developerworks", "cn", "linux" ]
	host = ""

	def check_page_chara(self, uri, page_chara):
		len1 = len(page_chara)
		len2 = len(uri)
		if len2 < len1:
			return 1
		for i in range(0, len1-1):
			if (page_chara[i] != uri[i]):
				return 1
			else:
				pass
		return 0

	def parse_listpage(self, response):
		print "listpage,url=%s" %(response.url)
		hxs = HtmlXPathSelector(response)
		next_page = hxs.select(r"//p[@class='ibm-table-navigation']/span[@class='ibm-primary-navigation']/span/a[@class='ibm-forward-em-link']")
		#print "nextpage.href=%s" %(next_page.select(r"@href").extract())
		#print "nextpage.text()=%s" %(next_page.select(r"text()").extract())
		page_list = hxs.select(r"//div[@class='ibm-container ibm-inner-data-table-last']/div[@class='ibm-container-body']/table/tbody/tr/td/a")
		href_lists = page_list.select(r'@href').extract()
		title_lists = page_list.select(r'strong/text()').extract()
		list_num = len(href_lists)
		for i in range(0, list_num-1):
			#print "title=%s href=%s" %(title_lists[i], href_lists[i])
			#url = "http://" + self.host + href_lists[i]
			time.sleep(1)
			yield Request(href_lists[i])
			
	def parse_contentpage(self, response):
		hxs = HtmlXPathSelector(response)
		print "url=%s" %(response.url)
		content = hxs.select(r"//div[@id='ibm-pcon']/div[@id='ibm-content']")
		summary = content.select("div[@id='dw-summary-article']")
		article_name = summary.select("div[@class='dw-content-head']/h1/text()").extract()
		print "\t to store file[%s]" %(article_name[0])
		open("linux_articles/"+article_name[0], "wb").write(response.body)
		return None

		main_content = content.select("div[@id='ibm-content-body']/div[@id='ibm-content-main']/div[@class='ibm-container']")
		#print "main_content %s" %main_content
		sum_text = summary.select("//h1|p|strong/text()").extract()
		for i in range(0, len(sum_text)-1):
			print "\t%s" %(sum_text[i])
		ctt_text = main_content.select("//pre|p|a|span")
		for i in range(0, len(ctt_text)-1):
			print u"\tctt = %s" %(ctt_text[i].select("text()").extract())
	
	def parse(self, response):
		url = response.url
		url = url.split(r"http://")[-1]
		url_segs = url.split(r"/")
		self.host = url_segs[0]
		res_name = url_segs[-1]
		uris = url_segs[1:len(url_segs)-1:]

		if self.check_page_chara(uris, self.listpage_chara) == 0:
			return self.parse_listpage(response)
		elif self.check_page_chara(uris, self.contentpage_chara) == 0:
			self.parse_contentpage(response)
		else:
			pass
