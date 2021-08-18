package Report1;

import java.io.IOException;
import java.util.Iterator;
import java.util.Scanner;

import org.jsoup.Jsoup; // Jsoup를 이용해서 크롤링
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;


class URL { // URL들 저장 클래스
	static String[] url = {
			"https://www.genie.co.kr/chart/musicHistory?year=2000&category=0", // genie뮤직 2000년도 1~50위
			"https://www.genie.co.kr/chart/musicHistory?year=2000&category=0&pg=2", // genie뮤직 2000년도 51~100위
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
	static int[] check = new int [URL.url.length]; //검색결과가 없는 년도를 체크하기 위한 전역변수
	
	public static void main(String[] args) throws InterruptedException { //main class이자 메인 쓰레드
		int num = URL.url.length; // 저장된 URL의 갯수
		Document[] doc = new Document[num]; // Document에는 페이지의 전체 소스가 저장된다
		System.out.println("input 된 URL의 수 : " + num); 

		try { //doc 변수에 URL을 넣어준다.
			System.out.println("Loading... URL을 불러오는 중입니다...");
			for (int i = 0; i < num; i++) {
				doc[i] = Jsoup.connect(URL.url[i]).get();
				System.out.print(i+1 + "..");
			}
		} catch (IOException e) {
			e.printStackTrace();
		}

		System.out.println("번째  URL 완료!");
		System.out.println("============================================================");
		System.out.println("Genie뮤직  2000 ~ 2019년 연도별  TOP100 차트에서 검색을 실행합니다");
		
		int a = 1;
		Scanner input = new Scanner(System.in);
		String keyword;
		Thread[] th = new Thread[num]; //멀티쓰레드 배열 생성, 삽입해준 URL의 수만큼 생성한다.

		while (a == 1) { //검색을 여러번 반복한다
			System.out.println("\n검색할 아티스트를 입력하세요: ");
			keyword = input.nextLine(); //검색할 단어
			
			for (int i = 0; i < num; i++) { //쓰레드들에 Runnable을 통해 파라미터를 넘겨준다
				check[i]=0;
				th[i] = new Thread(new Searcher(doc[i], keyword, i)); //쓰레드 생성 (URL주소, 찾는 단어, index)
				th[i].start();
			}

			for (int i = 0; i < num; i++) { //모든 쓰레들이 작업이 끝나길 기다린다
				th[i].join();
			}
			
			System.out.println("");
			for (int i = 0; i < num; i++) { //검색결과가 없는 년도를 출력
				if(check[i]==2)
					System.out.printf("20%02d년  ",i);
			}
			
			System.out.println("에는 " + keyword + "에 대한 검색결과가 없습니다.");
			System.out.println("\n======================== 검색 완료 ========================\n\n검색을 계속 하시겠습니까? 1.YES  2.NO");
			input = new Scanner(System.in);
			a = input.nextInt();
			input.nextLine();
		}		
		input.close();
		System.out.println("======================== 프로그램 종료  ========================");
	}

	
	public static class Searcher implements Runnable { //Runnable은 작업 내용만 가지고 있는 겍체

		private Document doc; // 검색을 실행할 URL
		private String keyword; // 검색할 단어
		private int index;
		
		public Searcher(Document doc, String keyword, int index) { //생성자
			this.doc = doc;
			this.keyword = keyword;
			this.index = index;
		}

		public void run() { // 이 스레드가 실제 실행할 코드
			if (doc.text().toUpperCase().contains(keyword.toUpperCase())) { //검색결과 포함 여부 판단, 대소문자무시
				Elements element = doc.select("#body-content > div.songlist-box > div.music-list-wrap > table > tbody"); //URL의 정보 추출
				Elements element2 = doc.select("#body-content > div.chart-date > div.date > h3"); //연도 추출
				
				Iterator<Element> el1 = element.select("a.artist.ellipsis").iterator(); //가수정보 저장
				Iterator<Element> el2 = element.select("a.title.ellipsis").iterator(); //타이틀 저장
				Iterator<Element> el3 = element.select("a.albumtitle.ellipsis").iterator(); //앨범 저장
				
				String title, artist;
				String year = element2.text();
				
				while (el1.hasNext()) {
					artist = el1.next().text();
					if (artist.toUpperCase().contains(keyword.toUpperCase())) { // 1~100위까지 차트에서 아티스트 포함 여부 판단, 대소문자 무시
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
			
			else { //검색한 단어가 없는 경우
				index/=2; //년도별로 페이지가 두개이므로
				check[index] += 1;
			}
		}
	}
}
