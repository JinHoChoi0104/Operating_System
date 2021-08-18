package Report1;

import java.io.IOException;
import java.util.Iterator;
import java.util.Scanner;

import org.jsoup.Jsoup; // Jsoup�� �̿��ؼ� ũ�Ѹ�
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;


class URL { // URL�� ���� Ŭ����
	static String[] url = {
			"https://www.genie.co.kr/chart/musicHistory?year=2000&category=0", // genie���� 2000�⵵ 1~50��
			"https://www.genie.co.kr/chart/musicHistory?year=2000&category=0&pg=2", // genie���� 2000�⵵ 51~100��
			"https://www.genie.co.kr/chart/musicHistory?year=2001&category=0",
			"https://www.genie.co.kr/chart/musicHistory?year=2001&category=0&pg=2",
			"https://www.genie.co.kr/chart/musicHistory?year=2002&category=0",
			"https://www.genie.co.kr/chart/musicHistory?year=2002&category=0&pg=2",
			"https://www.genie.co.kr/chart/musicHistory?year=2003&category=0",
			"https://www.genie.co.kr/chart/musicHistory?year=2003&category=0&pg=2",
			"https://www.genie.co.kr/chart/musicHistory?year=2004&category=0",
			"https://www.genie.co.kr/chart/musicHistory?year=2004&category=0&pg=2",
			"https://www.genie.co.kr/chart/musicHistory?year=2005&category=0",
			"https://www.genie.co.kr/chart/musicHistory?year=2005&category=0&pg=2",
			"https://www.genie.co.kr/chart/musicHistory?year=2006&category=0",
			"https://www.genie.co.kr/chart/musicHistory?year=2006&category=0&pg=2",
			"https://www.genie.co.kr/chart/musicHistory?year=2007&category=0",
			"https://www.genie.co.kr/chart/musicHistory?year=2007&category=0&pg=2",
			"https://www.genie.co.kr/chart/musicHistory?year=2008&category=0",
			"https://www.genie.co.kr/chart/musicHistory?year=2008&category=0&pg=2",
			"https://www.genie.co.kr/chart/musicHistory?year=2009&category=0",
			"https://www.genie.co.kr/chart/musicHistory?year=2009&category=0&pg=2",
			"https://www.genie.co.kr/chart/musicHistory?year=2010&category=0",
			"https://www.genie.co.kr/chart/musicHistory?year=2010&category=0&pg=2",
			"https://www.genie.co.kr/chart/musicHistory?year=2011&category=0",
			"https://www.genie.co.kr/chart/musicHistory?year=2011&category=0&pg=2",
			"https://www.genie.co.kr/chart/musicHistory?year=2012&category=0",
			"https://www.genie.co.kr/chart/musicHistory?year=2012&category=0&pg=2",
			"https://www.genie.co.kr/chart/musicHistory?year=2013&category=0",
			"https://www.genie.co.kr/chart/musicHistory?year=2013&category=0&pg=2",
			"https://www.genie.co.kr/chart/musicHistory?year=2014&category=0",
			"https://www.genie.co.kr/chart/musicHistory?year=2014&category=0&pg=2",
			"https://www.genie.co.kr/chart/musicHistory?year=2015&category=0",
			"https://www.genie.co.kr/chart/musicHistory?year=2015&category=0&pg=2",
			"https://www.genie.co.kr/chart/musicHistory?year=2016&category=0",
			"https://www.genie.co.kr/chart/musicHistory?year=2016&category=0&pg=2",
			"https://www.genie.co.kr/chart/musicHistory?year=2017&category=0",
			"https://www.genie.co.kr/chart/musicHistory?year=2017&category=0&pg=2",
			"https://www.genie.co.kr/chart/musicHistory?year=2018&category=0",
			"https://www.genie.co.kr/chart/musicHistory?year=2018&category=0&pg=2",
			"https://www.genie.co.kr/chart/musicHistory?year=2019&category=0",
			"https://www.genie.co.kr/chart/musicHistory?year=2019&category=0&pg=2"
	};
}

public class Crawling {
	static int[] check = new int [URL.url.length]; //�˻������ ���� �⵵�� üũ�ϱ� ���� ��������
	
	public static void main(String[] args) throws InterruptedException { //main class���� ���� ������
		int num = URL.url.length; // ����� URL�� ����
		Document[] doc = new Document[num]; // Document���� �������� ��ü �ҽ��� ����ȴ�
		System.out.println("input �� URL�� �� : " + num); 

		try { //doc ������ URL�� �־��ش�.
			System.out.println("Loading... URL�� �ҷ����� ���Դϴ�...");
			for (int i = 0; i < num; i++) {
				doc[i] = Jsoup.connect(URL.url[i]).get();
				System.out.print(i+1 + "..");
			}
		} catch (IOException e) {
			e.printStackTrace();
		}

		System.out.println("��°  URL �Ϸ�!");
		System.out.println("============================================================");
		System.out.println("Genie����  2000 ~ 2019�� ������  TOP100 ��Ʈ���� �˻��� �����մϴ�");
		
		int a = 1;
		Scanner input = new Scanner(System.in);
		String keyword;
		Thread[] th = new Thread[num]; //��Ƽ������ �迭 ����, �������� URL�� ����ŭ �����Ѵ�.

		while (a == 1) { //�˻��� ������ �ݺ��Ѵ�
			System.out.println("\n�˻��� ��Ƽ��Ʈ�� �Է��ϼ���: ");
			keyword = input.nextLine(); //�˻��� �ܾ�
			
			for (int i = 0; i < num; i++) { //������鿡 Runnable�� ���� �Ķ���͸� �Ѱ��ش�
				check[i]=0;
				th[i] = new Thread(new Searcher(doc[i], keyword, i)); //������ ���� (URL�ּ�, ã�� �ܾ�, index)
				th[i].start();
			}

			for (int i = 0; i < num; i++) { //��� �������� �۾��� ������ ��ٸ���
				th[i].join();
			}
			
			System.out.println("");
			for (int i = 0; i < num; i++) { //�˻������ ���� �⵵�� ���
				if(check[i]==2)
					System.out.printf("20%02d��  ",i);
			}
			
			System.out.println("���� " + keyword + "�� ���� �˻������ �����ϴ�.");
			System.out.println("\n======================== �˻� �Ϸ� ========================\n\n�˻��� ��� �Ͻðڽ��ϱ�? 1.YES  2.NO");
			input = new Scanner(System.in);
			a = input.nextInt();
			input.nextLine();
		}		
		input.close();
		System.out.println("======================== ���α׷� ����  ========================");
	}

	
	public static class Searcher implements Runnable { //Runnable�� �۾� ���븸 ������ �ִ� ��ü

		private Document doc; // �˻��� ������ URL
		private String keyword; // �˻��� �ܾ�
		private int index;
		
		public Searcher(Document doc, String keyword, int index) { //������
			this.doc = doc;
			this.keyword = keyword;
			this.index = index;
		}

		public void run() { // �� �����尡 ���� ������ �ڵ�
			if (doc.text().toUpperCase().contains(keyword.toUpperCase())) { //�˻���� ���� ���� �Ǵ�, ��ҹ��ڹ���
				Elements element = doc.select("#body-content > div.songlist-box > div.music-list-wrap > table > tbody"); //URL�� ���� ����
				Elements element2 = doc.select("#body-content > div.chart-date > div.date > h3"); //���� ����
				
				Iterator<Element> el1 = element.select("a.artist.ellipsis").iterator(); //�������� ����
				Iterator<Element> el2 = element.select("a.title.ellipsis").iterator(); //Ÿ��Ʋ ����
				Iterator<Element> el3 = element.select("a.albumtitle.ellipsis").iterator(); //�ٹ� ����
				
				String title, artist;
				String year = element2.text();
				
				while (el1.hasNext()) {
					artist = el1.next().text();
					if (artist.toUpperCase().contains(keyword.toUpperCase())) { // 1~100������ ��Ʈ���� ��Ƽ��Ʈ ���� ���� �Ǵ�, ��ҹ��� ����
						title = el2.next().text();
						title = title.replaceAll("TITLE ","");
						System.out.println("Year: "+ year + "    |  Artist: "+ artist + "  |  Title: " + title + "   |   Album: " + el3.next().text());
					}
					else {
						el2.next();
						el3.next();
					}
				}
			} 
			
			else { //�˻��� �ܾ ���� ���
				index/=2; //�⵵���� �������� �ΰ��̹Ƿ�
				check[index] += 1;
			}
		}
	}
}
