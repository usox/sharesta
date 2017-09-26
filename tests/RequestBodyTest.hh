<?hh // decl
namespace Usox\Sharesta;

use Facebook\TypeAssert\IncorrectTypeException;

class InputStreamWrapper {

	public static $singleton = null;
	private $index = 0;
	private $data = '';

	public function read($count){
		$length = min($count, strlen($this->data) - $this->index);
		$data = substr($this->data, $this->index);
		$this->index = $this->index + $length;
		return $data;
	}

	public function eof() {
		return $this->index >= strlen($this->data) ? TRUE : FALSE;
	}

	public function write($data){
		$this->index = 0;
		$this->data = $data;
	}

	public function stream_open($path, $mode, $options, &$opened_path){
		return true;
	}

	public function stream_close(){
	}

	public function stream_stat(){
		return array();
	}

	public function stream_flush(){
		return true;
	}

	private function getSingleton() {
		if (static::$singleton === null) {
			static::$singleton = new InputStreamWrapper();
		}
		return static::$singleton;
	}

	public function stream_read($count) {
		return $this->getSingleton()->read($count);
	}

	public function stream_eof() {
		return $this->getSingleton()->eof();
	}

	public function stream_write($data) {
		$this->getSingleton()->write($data);
	}

	public function unlink() {
	}
}

/**
 * Class: RequestBodyTest
 *
 * @see \PHPUnit_Framework_TestCase
 */
class RequestBodyTest extends \PHPUnit_Framework_TestCase {

	public function setUp() {
		stream_wrapper_unregister('php');
		stream_wrapper_register('php', InputStreamWrapper::class);
		$this->request_body = new RequestBody();
	}

	public function testWithoutJsonDataThrowsException() {
		$this->setExpectedException(
			Exception\RequestException::class
		);

		file_put_contents('php://input', 'absolutly-no-json');

		$this->request_body->getBody();
	}

	public function testWithEmptyBodyReturnsEmptyMap() {
		$this->assertEquals(
			Map{},
			$this->getRequestBody([])
		);
	}

	public function testWithJsonDataReturnsMap() {
		$request_data = ['data' => 'really'];
		$data = $this->getRequestBody($request_data);

		$this->assertEquals(
			new Map($request_data),
			$data
		);
	}

	public function testGetAsStringFailsWithNonString(): void {
		$this->expectException(IncorrectTypeException::class);

		$this->fillBody(['data' => 123]);

		$this->request_body->getAsString('data');
	}

	public function testGetAsStringReturnsString(): void {
		$expectation = 'some-string';

		$this->fillBody(['data' => $expectation]);

		$this->assertSame(
			$expectation,
			$this->request_body->getAsString('data')
		);
	}

	public function testGetAsIntFailsWithNonInt(): void {
		$this->expectException(IncorrectTypeException::class);

		$this->fillBody(['data' => 'we-want-snoo-snoo']);

		$this->request_body->getAsInt('data');
	}

	public function testGetAsIntReturnsInt(): void {
		$expectation = 123;

		$this->fillBody(['data' => $expectation]);

		$this->assertSame(
			$expectation,
			$this->request_body->getAsInt('data')
		);
	}

	public function testGetAsIntFailsWithNonBool(): void {
		$this->expectException(IncorrectTypeException::class);

		$this->fillBody(['data' => 'mister-noob-noob']);

		$this->request_body->getAsBool('data');
	}

	public function testGetAsBoolReturnsBool(): void {
		$expectation = true;

		$this->fillBody(['data' => $expectation]);

		$this->assertSame(
			$expectation,
			$this->request_body->getAsBool('data')
		);
	}

	public function testGetAsVectorThrowsExceptionOnNonArray(): void {
		$this->expectException(IncorrectTypeException::class);

		$this->fillBody(['data' => 'BOOM']);

		$this->request_body->getAsVector('data');
	}

	public function testGetAsVectorReturnsVector(): void {
		$data = ['I-will-be-ignored' => 'Me-not'];

		$this->fillBody(['data' => $data]);

		$this->assertEquals(
			new Vector($data),
			$this->request_body->getAsVector('data')
		);
	}

	public function testGetAsMapThrowsExceptionOnNonArray(): void {
		$this->expectException(IncorrectTypeException::class);

		$this->fillBody(['data' => 'BOOM']);

		$this->request_body->getAsMap('data');
	}

	public function testGetAsMapReturnsMap(): void {
		$data = ['da-key' => 'da-value'];

		$this->fillBody(['data' => $data]);

		$this->assertEquals(
			new Map($data),
			$this->request_body->getAsMap('data')
		);
	}

	private function fillBody(array<string, mixed> $body): void {
		file_put_contents('php://input', json_encode($body));
	}

	private function getRequestBody($input) {
		$this->fillBody($input);

		return $this->request_body->getBody();	
	}

	public function tearDown() {
		stream_wrapper_restore('php');
	}
}
