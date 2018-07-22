<?hh // strict
namespace Usox\Sharesta;

use function Facebook\FBExpect\expect;

class ResponseTest extends \Facebook\HackTest\HackTestCase {

	public function testSend500InvalidHttpStatus(): void {
		$response = new Response(666,' AGGIAGGI');

	}

	public function testSendReturnsDefaultBodyFor200(): void {
		$response = new Response(200, null);

		expect($this->getOutput($response))
		->toBeSame('"OK"');
	}

	public function testSendReturnsBodyWith200(): void {
		$body = 'my-body';
		$response = new Response(200, $body);

		expect($this->getOutput($response))
		->toBeSame($body);
	}

	private function getOutput(ResponseInterface $response): string {
		\ob_start();
		$response->send();
		$result = \ob_get_contents();
		\ob_end_clean();

		return $result;
	}
}
