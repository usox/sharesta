<?hh // strict
namespace Usox\Sharesta;

use function Facebook\FBExpect\expect;
use HH\Lib\{Str, Math};
use Facebook\TypeAssert\UnsupportedTypeException;

class RequestBodyTest extends \Facebook\HackTest\HackTest {

	<<__LateInit>> private RequestBody $request_body;


	private function setUp(): void {
		$this->request_body = new RequestBody();
	}

	public function testWithoutJsonDataThrowsException(): void {
    $this->fillBodyWithString('absolutly-no-json');

		expect(
			function () {
        $this->with();
			}
		)
		->toThrow(
			Exception\RequestException::class
		);
	}

	public function testWithEmptyBodyReturnsEmptyDict(): void {
		$this->setUp();

		expect($this->getRequestBody([]))->toBeSame(dict([]));
	}

	public function testWithJsonDataReturnsDict(): void {
		$this->setUp();
		$request_data = ['data' => 'really'];
		$data = $this->getRequestBody($request_data);

		expect($data)
		->toBeSame(dict($request_data));
	}

	public function testGetAsStringFailsWithNonString(): void {
		$this->fillBody(['data' => 123]);

		expect(
			() ==> $this->request_body->getAsString('data')
		)
		->toThrow(
			Exception\Request\InvalidRequestParamException::class
		);
	}

	public function testGetAsStringReturnsString(): void {
		$expectation = 'some-string';

		$this->fillBody(['data' => $expectation]);

		expect(
			$this->request_body->getAsString('data')
		)
		->toBeSame($expectation);
	}

	public function testGetAsIntFailsWithNonInt(): void {
		$this->fillBody(['data' => 'we-want-snoo-snoo']);

		expect(
			function() {
				$this->request_body->getAsInt('data');
			}
		)
		->toThrow(
			Exception\Request\InvalidRequestParamException::class
		);
	}

	public function testGetAsIntReturnsInt(): void {
		$expectation = 123;

		$this->fillBody(['data' => $expectation]);

		expect(
			$this->request_body->getAsInt('data')
		)
		->toBeSame($expectation);
	}

	public function testGetAsIntFailsWithNonBool(): void {
		$this->fillBody(['data' => 'mister-noob-noob']);

		expect(
			function() {
				$this->request_body->getAsBool('data');
			}
		)
		->toThrow(
			Exception\Request\InvalidRequestParamException::class
		);
	}

	public function testGetAsBoolReturnsBool(): void {
		$expectation = true;

		$this->fillBody(['data' => $expectation]);

		expect(
			$this->request_body->getAsBool('data')
		)
		->toBeSame($expectation);
	}

	public function testGetAsVecThrowsExceptionOnNonArray(): void {
		$this->fillBody(['data' => 'BOOM']);

		expect(() ==> $this->request_body->getAsVec('data'))
		->toThrow(
			Exception\Request\InvalidRequestParamException::class
		);
	}

	public function testGetAsVecReturnsVector(): void {
		$data = ['I-will-be-ignored' => 'Me-not'];

		$this->fillBody(['data' => $data]);

		expect(
			$this->request_body->getAsVec('data')
		)
		->toBeSame(vec($data));
	}

	public function testGetAsDictThrowsExceptionOnNonArray(): void {
		$this->fillBody(['data' => 'BOOM']);

		expect(
			function () {
				$this->request_body->getAsDict('data');
			}
		)
		->toThrow(
			Exception\Request\InvalidRequestParamException::class
		);
	}

	public function testGetAsDictReturnsDict(): void {
		$data = ['da-key' => 'da-value'];

		$this->fillBody(['data' => $data]);

		expect(
			$this->request_body->getAsDict('data')
		)
		->toBeSame(dict($data));
	}

	private function getRequestBody(array<string, mixed>$input): dict<string, mixed> {
		$this->fillBody($input);

		return $this->with();	
	}

	private function fillBody(array<string, mixed> $body): void {
		 $this->fillBodyWithString(\json_encode($body));
	}

  private function fillBodyWithString(string $input): void{
    $this->setUp();
    $this->request_body->useIO(new _Private\MockIOHandle($input));
  }

  private function with(): dict<string, mixed>{
    return $this->request_body->getBody();
  }
}
